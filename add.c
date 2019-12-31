#include "wrapper.h"

int main(void) {
    char *buf, *p;
    char content[MAXLINE];
    int n1=0, n2=0;

    /* Extract the two arguments from standard input */
    scanf("n1=%d&n2=%d", &n1, &n2);
    
    /* Make the response body */
    sprintf(content, "Test: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe addition is: %d + %d = %d\r\n<p>", 
	    content, n1, n2, n1 + n2);
    sprintf(content, "%sThe subtraction is: %d - %d = %d\r\n<p>", 
	    content, n1, n2, n1 - n2);
    sprintf(content, "%sThanks you watching!\r\n", content);

    /* Generate the HTTP response */
    printf("Content-length: %d\r\n", strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
