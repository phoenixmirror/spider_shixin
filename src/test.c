#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_BUF     655360
char wr_buf[MAX_BUF+1];
int  wr_index;

void usage()
{
        printf( "USAGE : \n" );
        printf( "        postreq ÍøÖ· µÇÂ½Êý¾Ý \n" );
        return ;
}

FILE *fp;
/*
* Write data callback function (called within the context of
* curl_easy_perform.
*/
size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp )
{
        int segsize = size * nmemb;

        /* Check to see if this data exceeds the size of our buffer. If so,
        * set the user-defined context value and return 0 to indicate a
        * problem to curl.
        */
        if ( wr_index + segsize > MAX_BUF )
        {
                *(int *)userp = 1;
                return 0;
        }

        /* Copy the data from the curl buffer into our buffer */
        memcpy( (void *)&wr_buf[wr_index], buffer, (size_t)segsize );

        /* Update the write index */
        wr_index += segsize;

        /* Null terminate the buffer */
        wr_buf[wr_index] = 0;

        /* Return the number of bytes received, indicating to curl that all is okay */
	printf("segsize=%d\n",segsize);
	fwrite(wr_buf, sizeof(char), segsize, fp );	
        return segsize;
}



int post_req( char *website, char *postdata )
{
        CURL            *curl;
        CURLcode        ret;
        int             wr_error = 0;
        FILE            *fp = NULL;
        wr_index = 0;

        curl = curl_easy_init();
        if( !curl )
        {
                printf("couldn't init curl\n");
                return -1;
        }

        curl_easy_setopt( curl, CURLOPT_URL, "zhixing.court.gov.cn/search/security/jcaptcha.jpg");
        /* curl_easy_setopt( curl, CURLOPT_URL, website );*/
        /* curl_easy_setopt( curl,  CURLOPT_COOKIEJAR, "/tmp/cookies.txt" ); */

        /* Tell curl that we'll receive data to the function write_data, and
        * also provide it with a context pointer for our error return.
        */
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&wr_error );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );

        /* strcpy( postdata, "j_username=13611884258&j_password=XZ6fek1sn%2FKjw9vWQJfrkf9PVtk2%2F800THQTwXZxDjwGs1lpCU%2FfRJavPFf8JtEbsIQGYTa3diAzH99fuNqWCYOqFsx5n041fn43KnUf4DipUFl%2F70U4BO4xMXHxZaM8BQDTOSYOdsDlGmkZAIcrwM3MQtugxSdBIAVLWD5MlTI%3D&rememberme=on&targetUrl=http%3A%2F%2Fwww.renrendai.com%2F&returnUrl="); */
        /*curl_easy_setopt( curl, CURLOPT_POSTFIELDS, postdata );*/
        /*curl_easy_setopt( curl, CURLOPT_POSTFIELDS, "currentPage=2  ");*/
        curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0");
        ret = curl_easy_perform( curl );
        printf( "ret = %d (write_error = %d)\n", ret, wr_error );

        /* Emit the page if curl indicates that no errors occurred */
        if ( ret == 0 )
        {
                printf("%s\n",wr_buf);
        }



        curl_easy_cleanup( curl );


        return ret;
}



int main( int argc, char *argv[] )
{
        int     nret = 0;

        if( argc < 3 )
        {
                usage();
                return  -1;
        }

	fp=fopen("/home/idd/111", "w");

        nret = post_req( argv[1], argv[2] );
        if( nret )
        {
                printf( "check in error\n" );
                return  nret;
        }

	fclose(fp);

        return 0;
}



