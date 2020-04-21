FILE* open_file(char *file,char *permissions);
void send_file(FILE *fd,int sockfd);
void recv_file(FILE *fd,int sockfd);
void CreatePackageList();
