#include "common.h"

DList *DList_new(DList *list)
{       
        int     i;
        
        if (list==NULL)
                list=(DList *)malloc(sizeof(DList));
        if (list!=NULL)
        {       list->head=NULL;
                list->cnt=0;
                for(i=0; i<DLIST_MAX_STEPD; ++i)
                {       list->use[i]=NULL;
                        list->useF[i]=0;
                };
        };
        return(list);
}               


                
int DList_add(DList *list,char *userData, char *userContent, int depth, int (*addFunc)(char *userData))
{
        DListNode       *p;
        int             err;

	if( userData==NULL || strlen(userData) <= 0 )
		return -1;

        p=list->head;
        while(p!=NULL)
                if( strcmp(p->userData, userData) == 0 )
                                return(DLIST_DUP_USERDATA);
                else p=p->n;

        if (addFunc!=NULL)
        {       err=(*addFunc)(userData);
                if (err!=0)
                        return(err);
        };

        p=(DListNode *)malloc(sizeof(DListNode));
        if (p==NULL)
                return(DLIST_NOMEMORY);

        /*p->userData=userData;*/
        strcpy( p->userData, userData );
	if( userContent != NULL && strlen(userContent)>0 )
		strcpy( p->userContent, userContent );
	else
		memset(p->userContent, 0x00, sizeof(p->userContent));
	p->depth = depth;
        p->n=list->head;
        if (list->head!=NULL)
                list->head->f=p;
        p->f=NULL;
        list->head=p;
        ++list->cnt;
	p->index = list->cnt;
        return(0);
}



int DList_clear(DList *list,int (*clearFunc)(char *userData))
{
        DListNode *p,*p1;
        int       err;

        p=list->head;
        while(p!=NULL)
        {
                p1=p->n;
                if (clearFunc!=NULL)
                {       err=(*clearFunc)(p->userData);
                        if (err!=0)
                        {       p->f=NULL;
                                list->head=p;
                                return(err);
                        };
                };
                --list->cnt;

                {int i;

                        for(i=0; i<DLIST_MAX_STEPD; ++i)
                                if ((list->useF[i]!=0)&&(list->use[i]==p))
                                        list->use[i]=list->use[i]->n;
                };
                free(p);
                p=p1;
        };
        list->head=NULL;
        return(0);
}



int DList_del(DList *list,char *userData,int (*delFunc)(void *userData))
{
        DListNode *p;
        int             err=0;

        p=list->head;
        while(p!=NULL)
                /* if (p->userData!=userData) */
                if ( strcmp( p->userData, userData ) != 0 )
                        p=p->n;
                else
                {
                        if (delFunc!=NULL)
                        {       err=(*delFunc)(userData);
                                if (err!=0)
                                        return(err);
                        };

                {int i;

                        for(i=0; i<DLIST_MAX_STEPD; ++i)
                                if ((list->useF[i]!=0)&&(list->use[i]==p))
                                        list->use[i]=list->use[i]->n;
                };

                        if (p->f==NULL)
                                        list->head=p->n;
                                else    p->f->n=p->n;
                        if (p->n!=NULL)
                                        p->n->f=p->f;
                        list->cnt--;
                        free(p);
                        return(0);
                };
        return(DLIST_NOTFOUND);
}



int DList_check(DList *list,char *userData,int (*checkFunc)(void *userData))
{
        DListNode       *p;
        int             err=0;

        p=list->head;
        while(p!=NULL)
                /* if (p->userData!=userData) */
                if ( strcmp( p->userData, userData ) == 0 )
                        p=p->n;
                else
                {
                        if (checkFunc!=NULL)
                                err=(*checkFunc)(userData);
                        return(err);
                };
        return(DLIST_NOTFOUND);
}



int DList_step(DList *list,int (*stepFunc)(void *userData))
{
        DListNode *p;
        int       err;
        int       i;
        int       j;

        for(i=-1,j=0; j<DLIST_MAX_STEPD&&i==-1; ++j)
                if (list->useF[j]==0)
                        i=j;

        if (i==-1)
                return(DLIST_STEPERROR);

        list->useF[i]=1;
        if (stepFunc!=NULL)
        {       p=list->head;
                while(p!=NULL)
                        {       list->use[i]=p->n;
                                err=(*stepFunc)(p->userData);
                                if (err!=0)
                                {       list->use[i]=NULL;
                                        list->useF[i]=0;
                                        return(err);
                                };
                                p=list->use[i];
                        };
        };
        list->useF[i]=0;
        return(0);
}




int DList_step2(DList *list,int (*step2Func)(void *userData,void *stepData),
                        void *stepData)
{
        DListNode *p;
        int       err;
        int       i;
        int       j;

        for(i=-1,j=0; j<DLIST_MAX_STEPD&&i==-1; ++j)
                if (list->useF[j]==0)
                        i=j;

        if (i==-1)
                return(DLIST_STEPERROR);

        list->useF[i]=1;
        if (step2Func!=NULL)
        {       p=list->head;
                while(p!=NULL)
                        {       list->use[i]=p->n;
                                err=(*step2Func)(p->userData,stepData);
                                if (err!=0)
                                {       list->use[i]=NULL;
                                        list->useF[i]=0;
                                        return(err);
                                };
                                p=list->use[i];
                        };
        };
        list->useF[i]=0;
        return(0);
}



int DList_openStep(DList *list,DList_stepInfo *sinfo)
{
        int       i;
        int       j;

        for(i=-1,j=0; j<DLIST_MAX_STEPD&&i==-1; ++j)
                if (list->useF[j]==0)
                        i=j;

        if (i==-1)
                return(DLIST_STEPERROR);

        *sinfo=i;
        list->useF[*sinfo]=1;
        list->use[*sinfo]=list->head;
        return(0);
}



int DList_fetchStep(DList *list,DList_stepInfo *sinfo,char *userDataP, char *userContentP, int *depth, int *index)
{
        DListNode *p;

        p=list->use[*sinfo];
        if (p!=NULL)
                list->use[*sinfo]=p->n;

        /* *userDataP=p->userData; */
        if (p!=NULL)
        {
                strcpy( userDataP, p->userData );
		if( userContentP != NULL && strlen(p->userContent)>0 )
			strcpy( userContentP, p->userContent );
		if( depth )
			*depth = p->depth;
		if( index )
			*index = p->index;
                return(0);
        }
        return(DLIST_NOTFOUND);
}


int DList_closeStep(DList *list,DList_stepInfo *sinfo)
{
        list->useF[*sinfo]=0;
        list->use[*sinfo]=NULL;
        return(0);
}





