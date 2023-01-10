#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dirent.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long long int initialize(int);
void start(int);
const char* generate_token(char*);
int maxx(int, int);
char* replaceWord(const char* , const char* ,const char* );
char* replaceWord1(const char* s, const char* oldW,const char* newW);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char buffer[1000];
int tokk = 0;
char** token1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    token1 = (char **) calloc(10, sizeof(char *));
    for(int g = 0; g < 10; g++){
        *(token1 + g) = (char*) calloc(40, sizeof(char));
    }
    int port = 12345;
    long long int server_socket = initialize(port);
    if(server_socket != INVALID_SOCKET){
        start(server_socket);
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long long int initialize(int port){
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if(wsa_res != 0){
        printf("Failed to run wsadata");
        return INVALID_SOCKET;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET){
        printf("Could not creat socket");
        return INVALID_SOCKET;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_res == SOCKET_ERROR){
        printf("Bind failed");
        return INVALID_SOCKET;
    }

    listen(server_socket, SOMAXCONN);

    return server_socket;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void start(int server_socket){
    int len = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client_addr;
    char *messages[1000];
    int size = 0;
    printf("Initializing...\n");
    printf("Listening on port 12345\n");
    while((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len)) != INVALID_SOCKET) {
        int repeated_username = 0;
        printf("Client connected\n");
        memset(buffer, 0, 1000);
        recv(client_socket, buffer, 999, 0);
        printf("\"Request\":%s", buffer);
        char *response = (char *) calloc(400, sizeof(char));
        int pointer = 0;
        int signup_flag = 0;
///////////////////////////////////////////////////////////sign up/////////////////////////////////////////////////////////////////////////
        if(strncmp(buffer, "signup", 6) == 0){
            printf("****************************************\n");
            printf("Attempting to signup\n");
            char *token;
            char *username = (char*) calloc(40, sizeof(char));
            memset(username, 0, 40);
            char *pass = (char*) calloc(40, sizeof(char));
            memset(pass, 0, 40);
            char username1[100];
            token = strtok(buffer, "\" ,:");
            int count = 0;
            while( token != NULL ){
                if(strncmp(token, "signup", 6) == 0){
                    signup_flag++;
                    pointer = count;
                }
                else if(signup_flag == 1 && count > pointer){
                    signup_flag = 0;
                    strcpy(username, token);
                    strcpy(username1, username);
                    strcat(username1, ".txt");
                    DIR *d;
                    struct dirent *dir;
                    d = opendir("./Resources/users");
                    int find = 0;
                    if (d){
                        while ((dir = readdir(d)) != NULL){
                            if(strcmp(dir->d_name, username1) == 0){
                                find++;
                            }
                        }
                        closedir(d);
                    }
                    ///check if this username exists
                    if(find != 0){
                        repeated_username = 1;
                        response = "{\"type\":\"Error\",\"message\":\"This username is already taken.\"}";
                        printf("\"Answer\":{\"type\":\"Error\",\"message\":\"This username is already taken.\"}\n");
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                }
                if(count == 2 && repeated_username == 0){
                    strcpy(pass, token);
                }
                token = strtok(NULL, "\" ,:");
                count++;
            }
            ///save new user in database if it is not repeated
            if(repeated_username == 0){

                char seen_locate[100];
                strcpy(seen_locate, "./Resources/seen/");
                strcat(seen_locate, username1);
                FILE *filePointer44;
                filePointer44 = fopen(seen_locate, "w");

                FILE *filePointer;
                char data[200]={};
                sprintf(data, "{\"username\":\"%s\"", username);
                strcat(data, ",\"password\":\"");
                strcat(data, pass);
                data[strcspn(data, "\n")] = 0;
                strcat(data, "\",\"bio\":\"\",\"followers\":[],\"followings\":[],\"personalTweets\":[]}");
                char* location = calloc(40, sizeof(char));
                memset(location, 0, 40);
                strcpy(location, "./Resources/users/");
                strcat(location, username1);
                filePointer = fopen(location, "w");
                if ( filePointer == NULL ){
                    printf( "file failed to open." );
                }
                else{
                    if(strlen (data) > 0){
                        fputs(data, filePointer) ;
                        fputs("\n", filePointer) ;
                    }
                    fclose(filePointer);
                }
                printf("\"Answer\":{\"type\":\"Successful\",\"message\":\"\"}\n");
                response = "{\"type\":\"Successful\",\"message\":\"\"}";
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
        }

///////////////////////////////////////////////////////////////////////////login/////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "login", 5) == 0){
            printf("****************************************\n");
            printf("Attempting to login\n");
            int count1 = 0;
            char * username = (char*) calloc(40, sizeof(char));
            char * username1 = (char*) calloc(40, sizeof(char));
            char * password = (char*) calloc( 40, sizeof(char));
            int fl = 0;
            int h = 0;
            char* aaa = strtok(buffer, "\" ,:");
            int already_logged = 0;
            int find1 = 0;
            while (aaa != NULL) {
                if(strncmp(aaa, "login", 5) == 0){
                    fl = 1;
                }
                else if(fl == 1){
                    fl = 0;
                    strcpy(username, aaa);
                    printf("aa is %s\n");
                    //printf("username is %s\n", username);
                    strcpy(username1, username);
                    printf("username1 is %s\n", username1);
                    strcat(username, ".txt");
                    DIR *d1;
                    struct dirent *dir;
                    d1 = opendir("./Resources/users");

                    if (d1){
                        while ((dir = readdir(d1)) != NULL){
                            if(strcmp(dir->d_name, username) == 0){
                                find1++;
                            }
                        }
                        closedir(d1);
                    }
                    /////check if this username is valid
                    if(find1 == 0){
                        strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is not valid.\"}");
                        printf("\"Message\": {\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                    ///searching if this username is already logged in
                    else{
                        username1[strlen(username1)]='\0';
                        for(int z = 0; z < tokk; z++){
                            if (strstr(token1[z],username1)){
                                already_logged = 1;
                            }

                         }
                    }
                    if(already_logged != 0){
                        strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is already signed in\"}");
                        printf("\"Message\":{\"type\":\"Error\",\"message\":\"This username is already signed in\"}\n");
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                }
                else if(already_logged == 0 && find1 != 0){
                    //printf("We are checking your entered password\n");
                    ////check if the password is correct
                    //memset(password, 0, 40);
                    printf("memseted password!\n");
                    sprintf(password, "\"password\":\"%s", aaa);
                    password[strcspn(password, "\n")] = 0;
                    strcat(password, "\"");
                    //printf("initializing password!\n");
                    printf("Entered password is %s\n", password);
                    FILE *filePointer ;
                    char dataToBeRead[1000];
                    char new1[1000]={};
                    char new2[1000] = {};
                    char* location1 = (char*) calloc(100, sizeof(char));
                    strcpy(location1, "./Resources/users/");
                    strcat(location1, username);
                    filePointer = fopen(location1, "r") ;
                    if ( filePointer == NULL ){
                        printf( "file failed to open." ) ;
                    }
                    else{
                        while( fgets ( dataToBeRead, 50, filePointer ) != NULL ){
                            sprintf(new1, "%s", dataToBeRead);
                            strcat(new2, new1);
                        }
                        fclose(filePointer) ;
                    }
                    int yy;
                    printf("checking for finding password\n");
                    password[strlen(password)]='\0';
                    if (strstr(new2,password)){
                        yy = 0;
                    }
                    else{
                        yy = -1;
                    }
                    ///correct password
                    //printf("checking if your password is correct\n");
                    if(yy == 0){
                        strcpy(token1[tokk], generate_token(username1));
                        memset(response, 0, 200);
                        strcat(response, "{\"type\":\"Token\",\"message\":\"");
                        strcat(response, token1[tokk]);
                        strcat(response, "\"}");
                        strcat(token1[tokk], ",");
                        //printf("Your username is %s\n", username1);
                        strcat(token1[tokk], username1);
                        printf("Your token and username are %s\n", token1[tokk]);
                        tokk++;
                        printf("\"Message\":%s\n", response);
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                    ///incorrect password
                    else{
                        printf("\"Message\":{\"type\":\"Error\",\"message\":\"Incorrect Password.\"}\n");
                        response = "{\"type\":\"Error\",\"message\":\"Incorrect Password.\"}";
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                }
                aaa = strtok(NULL, "\", :");
                count1++;
            }
        }
//////////////////////////////////////////////////////////////////send tweet//////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "send tweet", 10) == 0){
            printf("****************************************\n");
            printf("Attempting to send tweet\n");
            char this_token1[100]={};
            char this_tweet1[100]={};
            char this_username[100]={};
            char *temp;
            temp = strtok(buffer, "\", ");
            int token_flag1= 0;
            int tweet_flag1 = 0;
            while (temp != NULL) {
                if(strncmp(temp, "tweet", 5) == 0){
                    token_flag1 = 1;
                }
                else if(token_flag1 == 1){
                    token_flag1 = 0;
                    strcpy(this_token1, temp);
                    tweet_flag1 = 1;
                }
                else if(tweet_flag1 == 1){
                    strcat(this_tweet1, temp);
                    strcat(this_tweet1, " ");
                }
                temp = strtok(NULL, "\", ");
            }
            printf("your token:%s\tyour new tweet:%s\n", this_token1, this_tweet1);
            char temp_user[100]={};
            for(int y = 0; y < tokk; y++){
                char tempo[100]={};
                strcpy(tempo, token1[y]);
                if(strstr(tempo, this_token1) != NULL){
                    strcpy(temp_user, tempo);
                }
            }
            //printf("%s\n", temp_user);
            char *splitor = (char*) calloc(25, sizeof(char));
            splitor = strtok(temp_user, "\",");
            int counter = 0;
            while( splitor != NULL ){
                if(counter == 1){
                    strcpy(this_username, splitor);
                    printf("this splitor is %s\n", splitor);
                    printf("this username is %s\n", this_username);
                }
                splitor = strtok(NULL, "\",");
                counter++;
            }

            int flag = 0;
            int count = 1;
            while(flag == 0){
                char b[100]={};
                char locate[100]="./Resources/tweets/";
                sprintf(b, "%d", count);
                strcat(b, ".txt");
                strcat(locate, b);
                FILE *filePointer ;
                filePointer = fopen(locate, "r") ;
                if ( filePointer == NULL ){
                    printf( "file failed to open." ) ;
                    flag++;
                }
                else{
                    count++;
                    fclose(filePointer);
                }
            }
             //////we want to read the previous informaation from database of this user
             //printf("we are setting a filepointer\n");
            FILE *filePointer14;
            char dataToBeRead11[100];
            char temp11[100]={};
            char old_user11[100]={};
            char locate11[100]={};
            //printf("we want to creat a location\n");
            strcpy(locate11, "./Resources/users/");
            char help[100];
            sprintf(help, "%d", count);
            //printf("help is %d\n", count);
            strcat(locate11, this_username);
            strcat(locate11, ".txt");
            //printf("i want to search for your last data\n");
            filePointer14 = fopen(locate11, "r") ;
            if ( filePointer14 == NULL ){
                printf( "file failed to open.\n" ) ;
            }
            else{
                while( fgets ( dataToBeRead11, 50, filePointer14 ) != NULL ){
                    sprintf(temp11, "%s", dataToBeRead11);
                    strcat(old_user11, temp11);
                }
                fclose(filePointer14) ;
            }
            old_user11[strcspn(old_user11, "\n")] = 0;
            printf("your old data is %s\n", old_user11);
            old_user11[strlen(old_user11)-1]=0;
            old_user11[strlen(old_user11)-1]=0;
            if(old_user11[strlen(old_user11)-1] != '[')
                strcat(old_user11, ",");
            strcat(old_user11, help);
            strcat(old_user11, "]}");
            printf("your new data is %s\n", old_user11);

            FILE *filePointer111;
            filePointer111 = fopen(locate11, "w");
            if ( filePointer111 == NULL ){
                printf( "file failed to open.\n" );
            }
            else{
                if(strlen (old_user11) > 0){
                    fputs(old_user11, filePointer111) ;
                    fputs("\n", filePointer111) ;
                }
                fclose(filePointer111);
            }
            /////////end of checking database//////
            char final_tweet[200]={};
            strcpy(final_tweet, "{\"id\":");
            printf("%d\n", count);
            FILE *filePointer11;
            char real_locate[100] = {};
            char num_temp[100];
            sprintf(num_temp, "%d", count);
            strcat(final_tweet, num_temp);
            strcat(final_tweet, ",\"author\":\"");
            strcat(final_tweet, this_username);
            strcat(final_tweet, "\",\"content\":\"");
            strcat(final_tweet, this_tweet1);
            final_tweet[strcspn(final_tweet, "\n")] = 0;
            strcat(final_tweet, "\",\"comments\":{},\"likes\":0,\"liked\":{}}");
            strcat(num_temp, ".txt");
            strcpy(real_locate, "./Resources/tweets/");
            strcat(real_locate, num_temp);
            printf("final tweet up to now is %s\n", final_tweet);
            filePointer11 = fopen(real_locate, "w");
            if ( filePointer11 == NULL ){
                printf( "file failed to open.\n" );
            }
            else{
                if(strlen (final_tweet) > 0){
                    fputs(final_tweet, filePointer11) ;
                    fputs("\n", filePointer11) ;
                }
                fclose(filePointer11);
            }
            printf("\"Message\":{\"type\":\"Successful\",\"message\":\"Tweet is sent successfully.\"}\n");
            response = "{\"type\":\"Successful\",\"message\":\"Tweet is sent successfully.\"}";
            send(client_socket, response, strlen(response), 0);
            closesocket(client_socket);
        }
////////////////////////////////////////////////////////////log out//////////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "logout", 6) == 0) {
            printf("****************************************\n");
            printf("Attempting to logout\n");
            char *log_out_token = (char *) calloc(50, sizeof(char));
            char *qq = (char *) calloc(30, sizeof(char));
            qq = strtok(buffer, "\" ,");
            int flag = 0;
            while (qq != NULL) {
                if (strncmp(qq, "logout", 6) == 0) {
                    flag = 1;
                }
                else if (flag == 1) {
                    flag = 0;
                    strcpy(log_out_token, qq);
                }
                qq = strtok(NULL, "\" ,");
            }
            int logged1 = 0;
            char *search = (char *) calloc(50, sizeof(char));
            for (int z1 = 0; z1 < tokk; z1++) {
                memset(search, 0, 50);
                strcpy(search, token1[z1]);
                log_out_token[strlen(log_out_token)-1] = '\0';
                search[strlen(search)-1] = '\0';
                if (strstr(search, log_out_token)) {
                    logged1++;
                    free(token1[z1]);
                    printf("I deleted your token\n");
                }
            }
            if(logged1 != 0){
                printf("\"Message\":{\"type\":\"Successful\",\"message\":\"\"}\n");
                strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            printf("Logged out\n:)\n");
        }

//////////////////////////////////////////////////////set bio////////////////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "set bio", 7) == 0){
            printf("****************************************\n");
            char user_token[100];
            int flag_user_token = 0;
            char new_bio[100];
            int flag_new_bio = 0;
            char* replacer = (char*) calloc(100, sizeof(char));
            memset(replacer, 0, 100);
            replacer = strtok(buffer, "\" ,:");
            while (replacer != NULL) {
                if(strncmp(replacer, "bio", 3) == 0){
                    flag_user_token = 1;
                }
                else if(flag_user_token == 1){
                    flag_user_token = 0;
                    flag_new_bio = 1;
                    strcpy(user_token, replacer);
                }
                else if(flag_new_bio == 1){
                    flag_new_bio = 0;
                    strcpy(new_bio, replacer);
                }
                replacer = strtok(NULL, "\" ,:");
            }
            printf("Your sent token is %s\n", user_token);
            printf("Your sent new bio is %s\n", new_bio);
            user_token[strlen(user_token)] = '\0';
            char token2[100];
            char real_token[100];
            int find_token = 0;
            for(int m1 = 0; m1 < tokk; m1++){
                strcpy(token2, token1[m1]);
                if (strstr(token2, user_token)) {
                    find_token++;
                    strcpy(real_token, token2);
                }
            }
            char real_username[100];
            if(find_token != 0){
                printf("Your token and username are %s\n", real_token);
                char *replacer1;
                replacer1 = strtok(real_token, "\" ,:");
                int count = 0;
                while (replacer1 != NULL) {
                    if(count == 1){
                        strcpy(real_username, replacer1);
                    }
                    replacer1 = strtok(NULL, "\" ,:");
                    count++;
                }
            }
            char* h = (char*) calloc(100, sizeof(char));
            char location1[100];
            strcpy(location1, "./Resources/users/");
            strcat(location1, real_username);
            strcat(location1, ".txt");
            printf("your location is %s\n", location1);
            FILE *filePointer1;
            char dataToBeRead[100];
            char temp[100]={};
            char old_user[100];
            filePointer1 = fopen(location1, "r") ;
            if ( filePointer1 == NULL ){
                printf( "file failed to open.\n" ) ;
            }
            else{
                while( fgets ( dataToBeRead, 50, filePointer1 ) != NULL ){
                    sprintf(temp, "%s", dataToBeRead);
                    strcat(old_user, temp);
                    strcat(h, temp);
                }
                fclose(filePointer1) ;
            }
            printf("your old data is %s\n", old_user);
            char *replacer2;
            replacer2 = strtok(old_user, "\",:");
            char this_user[100];
            char this_pass[100];
            int flag_old_bio = 0;
            int set_new_bio = 0;
            int flag_exit = 0;
            int flag_username = 0;
            int flag_pass = 0;
            int cannot_update_bio = 0;
            while (replacer2 != NULL) {
                if(strncmp(replacer2, "username", 8) == 0){
                    flag_username = 1;
                }
                else if(flag_username == 1){
                    flag_username = 0;
                    strcpy(this_user, replacer2);
                }
                else if(strncmp(replacer2, "password", 8) == 0){
                    flag_pass = 1;
                }
                else if(flag_pass == 1){
                    flag_pass = 0;
                    strcpy(this_pass, replacer2);
                }
                else if(strncmp(replacer2, "bio", 3) == 0){
                    flag_old_bio = 1;
                }
                else if(flag_old_bio == 1){
                    flag_old_bio = 0;
                    if(strncmp(replacer2, "followers", 9) != 0){
                        cannot_update_bio++;
                        printf("\"Message\":{\"type\":\"Error\",\"message\":\"Bio is already updated\"}\n");
                        strcpy(response, "{\"type\":\"Error\",\"message\":\"Bio is already updated\"}");
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                    else if(flag_exit == 0 && set_new_bio == 0){
                        set_new_bio = 1;
                        flag_exit = 1;
                        printf("we are working on seting new bio\n");
                    }
                }
                replacer2 = strtok(NULL, "\",:");
            }
            if(cannot_update_bio == 0){
                char* f = "followers";
                //printf("%s\n", strstr(h,f));
                char* new_user_info = (char*) calloc(200, sizeof(char));
                strcpy(new_user_info, "{\"username\":\"");
                strcat(new_user_info, this_user);
                strcat(new_user_info, ",\"password\":\"");
                strcat(new_user_info, this_pass);
                strcat(new_user_info, "\",\"bio\":\"");
                strcat(new_user_info, new_bio);
                new_user_info[strcspn(new_user_info, "\n")] = 0;
                strcat(new_user_info, "\",\"");
                strcat(new_user_info, strstr(h, f));
                printf("%s\n", new_user_info);
                FILE *filePointer2;
                filePointer2 = fopen(location1, "w");
                if ( filePointer2 == NULL ){
                    printf( "file failed to open." );
                }
                else{
                    if(strlen (new_user_info) > 0){
                        fputs(new_user_info, filePointer2) ;
                        fputs("\n", filePointer2) ;
                    }
                    fclose(filePointer2);
                }
                printf("\"Answer\":{\"type\":\"Successful\",\"message\":\"Bio is updated successfully.\"}\n");
                response = "{\"type\":\"Successful\",\"message\":\"Bio is updated successfully.\"}";
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
        }
 //////////////////////////////////////////////////////////////refresh////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "refresh", 7) == 0){
            printf("****************************************\n");
            printf("we are checking for new tweets\n");
            /////
            ///1.(A)get the token and (B)split the username
            ///2.check for followings in the users
            ///3.check for(new) tweets of the followings
            ///4.copy new tweets based on their id
            ///send to client
            ////
            strcpy(response, "{\"type\":\"List\",\"message\":[");
            char all_of_new_tweets[600]={};
            char this_token1[100];
            char this_username1[100];
            ///1-A
            char* splitor111 = (char*) calloc(50, sizeof(char));
            splitor111 =strtok(buffer, "\", ");
            int flag5 = 0;
            while(splitor111 != NULL){
                if(strncmp(splitor111, "refresh", 7) == 0){
                    flag5++;
                }
                else if(flag5 == 1){
                    flag5 = 0;
                    strcpy(this_token1, splitor111);
                }
                splitor111 = strtok(NULL, "\", ");
            }
            this_token1[strcspn(this_token1,"\n")] = 0;
            //printf("this token1 is %s\n", this_token1);
            ///1-B
            char tempor[100]={};
            char username_token[100]={};
            for(int count1 = 0; count1 < tokk; count1++){
                strcpy(tempor, token1[count1]);
                if(strstr(tempor, this_token1) != NULL){
                    strcpy(username_token,tempor);
                }
            }
            //printf("this username and token are %s\n", username_token);
            char* splitor112 = (char*) calloc(50, sizeof(char));
            splitor112 = strtok(username_token, ",");
            int count = 0;
            while(splitor112 != NULL){
                if(count == 1){
                    strcpy(this_username1, splitor112);
                }
                count++;
                splitor112 = strtok(NULL, ",");
            }
            printf("this username is %s\n", this_username1);
            ///2 - check for followings
            char followings[20][20]={};
            char refresh_locate[100];
            strcpy(refresh_locate, "./Resources/users/");
            strcat(refresh_locate, this_username1);
            strcat(refresh_locate, ".txt");
            printf("this location is %s\n", refresh_locate);
            FILE *filePointer113;
            char dataToBeRead[100];
            char temp22[100]={};
            char data[100]={};
            filePointer113 = fopen(refresh_locate, "r") ;
            if ( filePointer113 == NULL ){
                printf( "file failed to open.\n" ) ;
            }
            else{
                while( fgets ( dataToBeRead, 50, filePointer113 ) != NULL ){
                    sprintf(temp22, "%s", dataToBeRead);
                    strcat(data, temp22);
                }
                fclose(filePointer113) ;
            }
            printf("your data in the database is %s\n", data);
            char* following_splitor = (char*) calloc(20, sizeof(char));
            following_splitor = strtok(data, "\",:[]{}");
            int following_flag = 0;
            int following_number = 0;
            while(following_splitor != NULL){
                if(strncmp(following_splitor, "followings", 10) == 0){
                    following_flag = 1;
                }
                else if(strncmp(following_splitor, "personalTweets", 14) == 0){
                    following_flag = 0;
                }
                else if(following_flag == 1){
                    //printf("you are following %s\n", following_splitor);
                    strcpy(followings[following_number], following_splitor);
                    following_number++;
                }
                following_splitor = strtok(NULL, "\",:[]}{");
            }

            FILE *filePointer55;
            char dataToBeRead11[100];
            char seen_locate1[100];
            char temp88[100];
            char before_seen[100]={};
            strcpy(seen_locate1, "./Resources/seen/");
            strcat(seen_locate1, this_username1);
            strcat(seen_locate1, ".txt");
            filePointer55 = fopen(seen_locate1, "r") ;
            if ( filePointer55 == NULL ){
                printf( "file failed to open.\n" ) ;
            }
            else{
                while( fgets ( dataToBeRead11, 50, filePointer55 ) != NULL ){
                    sprintf(temp88, "%s", dataToBeRead11);
                    strcat(before_seen, temp88);
                }
                fclose(filePointer55) ;
            }
            before_seen[strcspn(before_seen,"\n")] = 0;
            printf("before seen id's are %s\n", before_seen);
            char new_seen[100]={};
            strcpy(new_seen, before_seen);
            int number_of_before_seens = 0;
            char before_seen_ids[100][10]={};
            char* before_seen_splitor = (char*) calloc(40, sizeof(char));
            before_seen_splitor = strtok(before_seen, ",");
            while(before_seen_splitor != NULL){
                //printf("one of before seen id's is %s\n", before_seen_splitor);
                strcpy(before_seen_ids[number_of_before_seens], before_seen_splitor);
                number_of_before_seens++;
                before_seen_splitor = strtok(NULL, ",");
            }
//            for(int i = 0; i < number_of_before_seens; i++){
//                printf("one of before seens are %s\n", before_seen_ids[i]);
//            }

            int first = 0;
            for(int v = 0; v < following_number; v++){
                printf("you are following %s\n", followings[v]);
                FILE *filePointer114;
                char following_locate[100]={};
                strcpy(following_locate, "./Resources/users/");
                strcat(following_locate, followings[v]);
                strcat(following_locate, ".txt");
                printf("this following location is %s\n", following_locate);
                char dataToBeRead7[100];
                char temp227[100]={};
                char data7[100]={};
                filePointer114 = fopen(following_locate, "r") ;
                if ( filePointer114 == NULL ){
                    printf( "file failed to open.\n" ) ;
                }
                else{
                    while( fgets ( dataToBeRead7, 50, filePointer114) != NULL ){
                        sprintf(temp227, "%s", dataToBeRead7);
                        strcat(data7, temp227);
                    }
                    fclose(filePointer114) ;
                }
                printf("the content of this following is %s\n", data7);
                data7[strcspn(data7, "\n")] = 0;
                char* id_splitor = (char*) calloc(40, sizeof(char));
                id_splitor = strtok(data7, ",\":[]{} ");
                int id_flag = 0;
                while(id_splitor != NULL){
                    if(strncmp(id_splitor, "personalTweets", 14) == 0){
                        id_flag = 1;
                    }
                    else if(id_flag == 1){
                        int flag = 0;
                        printf("id is %s\n", id_splitor);
                        for(int h = 0; h < number_of_before_seens && flag == 0; h++){
                            if(strncmp(before_seen_ids[h], id_splitor,maxx(strlen(id_splitor), strlen(before_seen_ids[h]))) == 0){
                                printf("I have seen this before!!!\n");
                                flag = 1;
                            }
                            else{
                                strcpy(before_seen_ids[number_of_before_seens], id_splitor);
                                //printf("I will send the notification of this new id\n");
                            }
                        }
                        if(flag == 0){
                            char id_content555[200] = {};
                            char locate656[100] = {};
                            strcpy(locate656, "./Resources/tweets/");
                            strcat(locate656, id_splitor);
                            strcat(locate656, ".txt");
                            printf("location of this id is %s\n", locate656);
                            /////////////
                            FILE *filePointer656;
                            char temp656[200] = {};
                            char dataToBeRead656[200]= {};
                            filePointer656 = fopen(locate656, "r") ;
                            if ( filePointer656 == NULL ){
                                printf( "file failed to open.\n" ) ;
                            }
                            else{
                                while( fgets ( dataToBeRead656, 50, filePointer656) != NULL ){
                                    sprintf(temp656, "%s", dataToBeRead656);
                                    strcat(id_content555, temp656);
                                }
                                fclose(filePointer656) ;
                            }
                            /////////////////

                            if(first == 0){
                                id_content555[strcspn(id_content555,"\n")] = 0;
                                strcat(all_of_new_tweets, id_content555);
                            }
                            else{
                                id_content555[strcspn(id_content555,"\n")] = 0;
                                strcat(all_of_new_tweets, ",");
                                strcat(all_of_new_tweets, id_content555);
                            }
                            strcat(new_seen,",");
                            strcat(new_seen, id_splitor);
                            printf("new seen up to now is %s\n", new_seen);
                            //printf("i will send its notif\n");
                            number_of_before_seens++;
                            first++;
                        }
                    }
                    id_splitor = strtok(NULL, ",\":[]{} ");
                }
            }
            char change_seen_location[100];
            strcpy(change_seen_location, "./Resources/seen/");
            strcat(change_seen_location,this_username1);
            strcat(change_seen_location, ".txt");
            FILE *filePointer99;
            filePointer99 = fopen(change_seen_location, "w");
            if ( filePointer99 == NULL ){
                printf( "file failed to open." );
            }
            else{
                if(strlen (new_seen) > 0){
                    fputs(new_seen, filePointer99);
                    fputs("\n", filePointer99);
                }
                fclose(filePointer99);
            }
            ///3
            strcat(all_of_new_tweets,"]}");
            strcat(response, all_of_new_tweets);
            printf("response: %s\n", response);
            send(client_socket, response, strlen(response), 0);
            closesocket(client_socket);
        }
/////////////////////////////////////////////////////////////////////like////////////////////////////////////////////////////////////
        else if(strncmp("like", buffer, 4) == 0){
            printf("****************************************\n");
            printf("Attempting to like a tweet\n");
            ///
            ///1.get token and split the username and id
            ///2.find that id and like it
            ///3.send response
            ///
            char like_token[200]={};
            char like_username[200]={};
            char like_id[200]={};
            char id_content[200]={};
            ///
            char* like_split = (char*) calloc(60, sizeof(char));
            like_split = strtok(buffer, "\", ");
            int token_flag = 0;
            int id_flag = 0;
            while(like_split != NULL){
                if(strncmp("like", like_split, 4) == 0){
                    token_flag = 1;
                }
                else if(token_flag == 1){
                    token_flag = 0;
                    strcpy(like_token, like_split);
                    id_flag = 1;
                }
                else if(id_flag == 1){
                    id_flag = 0;
                    like_split[strcspn(like_split, "\n")] = 0;
                    strcpy(like_id, like_split);
                }
                like_split = strtok(NULL, "\", ");
            }
            like_token[strcspn(like_token, "\n")] = 0;
            printf("this id is %s\n", like_id);
            printf("this token is %s\n", like_token);
            char like_user_and_tok[200] ={};
            for(int c = 0; c < tokk; c++){
                char temp5[200]={};
                strcpy(temp5, token1[c]);
                if(strstr(temp5, like_token) != NULL){
                    strcpy(like_user_and_tok, temp5);
                }
            }
            printf("user and token are %s\n", like_user_and_tok);
            char* spl = (char*) calloc(60, sizeof(char));
            spl = strtok(like_user_and_tok, ",");
            int h6 = 0;
            while(spl != NULL){
                //printf("h6 is %d and spl is %s\n", h6, spl);
                if(h6 == 1){
                    strcpy(like_username, spl);
                }
                spl = strtok(NULL, ",");
                h6++;
            }
            //printf("username is %s\n", like_username);
            char id_locate[200]={};
            strcpy(id_locate, "./Resources/tweets/");
            strcat(id_locate, like_id);
            strcat(id_locate, ".txt");
            printf("id locate is %s\n", id_locate);

            FILE *filePointer114;
            char dataread[200];
            char temporar[200] = {};
            filePointer114 = fopen(id_locate, "r");
            if ( filePointer114 == NULL ){
                printf( "file failed to open.\n" );
            }
            else{
                while( fgets ( dataread, 50, filePointer114) != NULL ){
                    sprintf(temporar, "%s", dataread);
                    strcat(id_content, temporar);
                }
                fclose(filePointer114) ;
            }
            id_content[strcspn(id_content, "\n")] = 0;
            printf("id content is %s\n", id_content);
            char t[200];
            strcpy(t, id_content);
            char* sss = (char*) calloc(40 , sizeof(char));
            sss = strtok(id_content, "\",:}{");
            int likes_flag = 0;
            int liked_flag = 0;
            int we_have_liked_before = 0;
            int pre_num_likes = 0;
            while(sss != NULL){
                printf("sss is %s\n", sss);
                if(strncmp("likes", sss, 5) == 0){
                    likes_flag = 1;
                }
                else if(likes_flag == 1){
                    likes_flag = 0;
                    pre_num_likes = atoi(sss);
                    printf("number of pre likes is %d\n", pre_num_likes);
                }
                else if(strncmp("liked", sss, 5) == 0){
                    liked_flag = 1;
                }
                else if(liked_flag == 1){
                    printf("who liked this was %s\n", sss);
                    if(strncmp(like_username, sss, maxx(strlen(sss), strlen(like_username))) == 0){
                        printf("we have liked it before\n");
                        we_have_liked_before++;
                    }
                }
                sss = strtok(NULL, "\",:{}");
            }
            if(we_have_liked_before != 0){
                strcpy(response, "{\"type\":\"Error\",\"message\":\"You have liked this before.\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                char final1[100] = {};
                char* result = NULL;
                char c[100]={};
                strcpy(c, "likes\":");
                char q[100]={};
                sprintf(q, "%d", pre_num_likes);
                strcat(c, q);
                printf("c is %s\n", c);
                ///
                char d[100]={};
                strcpy(d, "likes\":");
                char u[100]={};
                sprintf(u, "%d", pre_num_likes+1);
                strcat(d, u);
                printf("d is %s\n", d);
                ///
                result = replaceWord(t, c, d);
                strcpy(final1, result);
                char* result2 = NULL;
                char before[100] = {};
                strcpy(before, "liked\":{");
                char after[100] = {};
                strcpy(after, "liked\":{\"");
                strcat(after, like_username);
                strcat(after, "\",");
                result2 = replaceWord(final1, before, after);
                strcpy(final1, result2);
                printf("last result is %s\n", final1);
                char* result3 = NULL;
                result3 = replaceWord(final1, ",}", "}");
                strcpy(final1, result3);
                printf("result is %s\n", final1);
//
//                if(result[strlen(rresult[strlen(result)-1] = 0;
//                result[strlen(result)-1] = 0;esult)-1] !='{')
//                    strcat(result, ",");
//                strcat(result, "\"");
//                strcat(result, like_username);
//                strcat(result, "\"}}");

                //free(result);
                printf("final is %s\n", final1);
                FILE *filePointer27;
                printf("id locate is %s\n", id_locate);
                filePointer27 = fopen(id_locate, "w");
                printf("continue\n");
                if ( filePointer27 == NULL ){
                    printf( "file failed to open." );
                }
                else{
                    printf("go\n");
                    if(strlen (final1) > 0){
                        //printf("i am in\n");
                        fputs(final1, filePointer27);
                        //printf("22\n");
                        fputs("\n", filePointer27) ;
                        printf("5454\n");
                    }
                    printf("fffd\n");
                    fclose(filePointer27);
                    printf("aaa\n");
                }
                //free(result);
                strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
        }
////////////////////////////////////////////////////////////////////////profile/////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "profile", 7) == 0){
            printf("****************************************\n");
            printf("Attempting for tweet profile\n");
            ////
            char username[100] = {};///
            char bio[100] = {};///
            int numberOfFollowers;///
            int numberOfFollowings;///
            char followstatus[100] = "Yourself";///
            char all_tweets[1000] = {};
            ////
            char my_token[100] = {};
            char my_token_and_user[100] = {};
            ////
            printf("start\n");
            char* split6 = (char*) calloc(50, sizeof(char));
            buffer[strlen(buffer)-1] = 0;
            split6 = strtok(buffer, ", ");
            int flag = 0;
            printf("1\n");
            int count11 = 0;
            while(split6 != NULL && count11 <= 1){
                printf("count is %d %s\n",  count11 ,split6);
                if(strncmp("profile", split6, 7) == 0){
                    flag = 1;
                }
                else if(flag == 1){
                    flag = 0;
                    strcpy(my_token, split6);
                    printf("split6 is %s\n", split6);
                }
                if(count11 <= 1)
                    split6 = strtok(NULL, ", ");
                count11++;
            }
            printf("token is %s\n", my_token);
            for(int g = 0; g < tokk; g++){
                char helpy[100] = {};
                strcpy(helpy, token1[g]);
                if(strstr(helpy, my_token) != NULL){
                    strcpy(my_token_and_user, helpy);
                    ///error if not exist
                }
            }
            printf("token and user are %s\n", my_token_and_user);
            char* split7 = (char*) calloc(50, sizeof(char));
            split7 = strtok(my_token_and_user, ",");
            int count = 0;
            while(split7 != NULL){
                if(count == 1){
                    strcpy(username, split7);
                }
                split7 = strtok(NULL, ",");
                count++;
            }
            printf("username is %s\n", username);
            char user_contents[200] = {};
            char my_location[100] = {};
            username[strcspn(username, "\n")] = 0;
            strcpy(my_location, "./Resources/users/");
            strcat(my_location, username);
            strcat(my_location, ".txt");
            printf("location is %s\n", my_location);
            FILE *filePointer44;
            char dataread[200];
            char temporar[200] = {};
            filePointer44 = fopen(my_location, "r");
            if ( filePointer44 == NULL ){
                printf( "file failed to open.\n" );
            }
            else{
                while( fgets ( dataread, 50, filePointer44) != NULL ){
                    sprintf(temporar, "%s", dataread);
                    strcat(user_contents, temporar);
                }
                fclose(filePointer44) ;
            }
            user_contents[strcspn(user_contents, "\n")] = 0;
            printf("user content is %s\n", user_contents);
            ///spliting the contents of this user and extracting the information
            int bio_flag = 0;
            int follower_flag = 0;
            int follower_count = 0;
            int following_flag = 0;
            int following_count = 0;
            int personal_tweets_flag = 0;
            int all_tweets_count = 0;
            char* profile_split = (char*) calloc(40, sizeof(char));
            profile_split = strtok(user_contents, "\"{},:[]");
            while(profile_split != NULL){
                printf("it is %s\n", profile_split);
                if(strncmp("bio", profile_split, 3) == 0){
                    bio_flag = 1;
                }
                else if(bio_flag == 1 && strncmp("followers", profile_split, 9) == 0){
                    bio_flag = 0;
                    follower_flag = 1;
                }
                else if(bio_flag == 1 && strncmp("followers", profile_split, 9) != 0){
                    bio_flag = 0;
                    strcpy(bio, profile_split);
                }
                else if(strncmp("followers", profile_split, 9) == 0){
                    follower_flag = 1;
                }
                else if(follower_flag == 1 && strncmp("followings", profile_split, 10) != 0){
                    follower_count++;
                }
                else if(follower_flag == 1 && strncmp("followings", profile_split, 10) == 0){
                    follower_flag = 0;
                    following_flag = 1;
                }
                else if(strncmp("followings", profile_split, 10) == 0){
                    following_flag = 1;
                }
                else if(following_flag == 1 && strncmp("personalTweets", profile_split, 14) != 0){
                    following_count++;
                }
                else if(following_flag == 1 && strncmp("personalTweets", profile_split, 14) == 0){
                    following_flag = 0;
                    personal_tweets_flag = 1;
                }
                else if(strncmp("personalTweets", profile_split, 14) == 0){
                    personal_tweets_flag = 1;
                }
                else if(personal_tweets_flag == 1){
                    //printf("this is a personal tweet %s\n", profile_split);
                    FILE *filePointerp;
                    char dataread[200];
                    char temporar[200] = {};
                    char this_tweet[100] = {};
                    char id_locate[100] = {};
                    strcpy(id_locate, "./Resources/tweets/");
                    strcat(id_locate, profile_split);
                    strcat(id_locate, ".txt");
                    //printf("this id location is %s\n", id_locate);
                    filePointerp = fopen(id_locate, "r");
                    if ( filePointerp == NULL ){
                        printf( "file failed to open.\n" );
                    }
                    else{
                        while( fgets ( dataread, 50, filePointerp) != NULL ){
                            sprintf(temporar, "%s", dataread);
                            strcat(this_tweet, temporar);
                        }
                        fclose(filePointerp);
                    }
                    if(all_tweets_count != 0){
                        strcat(all_tweets, ",");
                    }
                    strcat(all_tweets, this_tweet);
                    all_tweets[strcspn(all_tweets, "\n")] = 0;
                    all_tweets_count++;
                    //printf("this tweet content is %s\n", this_tweet);
                }
                profile_split = strtok(NULL, "\"{},:[]");
            }
            char num_follower[10]={};
            sprintf(num_follower, "%d", follower_count);
            char num_following[10]={};
            sprintf(num_following, "%d", following_count);
            //printf("number of followers is %s\n", num_follower);
            //printf("number of followings is %s\n", num_following);
            //printf("bio is %s\n", bio);
            ////
            strcpy(response, "{\"type\":\"Profile\",\"message\":{\"username\":\"");
            strcat(response, username);
            strcat(response, "\",\"bio\":\"");
            strcat(response, bio);
            strcat(response, "\",\"numberOfFollowers\":");
            strcat(response, num_follower);
            strcat(response, ",\"numberOfFollowings\":");
            strcat(response, num_following);
            strcat(response, ",\"followStatus\":\"");
            strcat(response, followstatus);
            strcat(response, "\",\"allTweets\":[");
            strcat(response, all_tweets);
            strcat(response, "]}}");
            printf("response: %s\n", response);
            send(client_socket, response, strlen(response), 0);
            closesocket(client_socket);
        }
/////////////////////////////////////////////////////change password//////////////////////////////////////////////////////////////////////////
        else if(strncmp("change password", buffer, 15) == 0){
            printf("****************************************\n");
            printf("Attempting to change password\n");
            ///
            ///1.split and extract token, current_pass and new_pass
            ///2.check if token exists and split username
            ///3.(?)check if current pass is correct
            ///4.(?)check if new pass is equal to current pass
            ///5.(?)change password in database
            ///6.send response
            ///
            char token[100] = {};
            char username[100] = {};
            char token_and_username[100] = {};
            char entered_current_pass[100] = {};
            char real_current_pass[100] = {};
            char entered_new_pass[100] = {};
            ///
            ///1.split and extract token, current_pass and new_pass
            char* splitor1 = (char*) calloc(30, sizeof(char));
            splitor1 = strtok(buffer, ",\" ");
            int token_flag = 0;
            int current_pass_flag = 0;
            int new_pass_flag = 0;
            while(splitor1 != NULL){
                if(strncmp("password", splitor1, 8) == 0){
                    token_flag = 1;
                }
                else if(token_flag == 1){
                    token_flag = 0;
                    strcpy(token, splitor1);
                    current_pass_flag = 1;
                }
                else if(current_pass_flag == 1){
                    current_pass_flag = 0;
                    strcpy(entered_current_pass, splitor1);
                    new_pass_flag = 1;
                }
                else if(new_pass_flag == 1){
                    new_pass_flag = 0;
                    strcpy(entered_new_pass, splitor1);
                }
                splitor1 = strtok(NULL, ",\" ");
            }
//            printf("token is %s\n", token);
//            printf("current is %s\n", entered_current_pass);
//            printf("new is %s\n", entered_new_pass);
            ///2.check if token exists and split username
            int token_exist = 0;
            for(int counter = 0; counter < tokk; counter++){
                char temp[100] = {};
                strcpy(temp, token1[counter]);
                if(strstr(temp, token) != NULL){
                    token_exist = 1;
                    strcpy(token_and_username, temp);
                }
            }
            //printf("username and token are %s\n", token_and_username);
            if(token_exist == 0){
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                char* splitor2 = (char*) calloc(30, sizeof(char));
                splitor2 = strtok(token_and_username, ",");
                int count2 = 0;
                while(splitor2 != NULL){
                    if(count2 == 1){
                        strcpy(username, splitor2);
                    }
                    splitor2 = strtok(NULL, ",");
                    count2++;
                }
                //printf("username is %s\n", username);
                ///3.(?)check if current pass is correct
                char user_location[100] = {};
                strcpy(user_location, "./Resources/users/");
                strcat(user_location, username);
                strcat(user_location, ".txt");
                printf("user location is %s\n", user_location);
                FILE *filePointer9;
                char dataread[200];
                char temporar[200] = {};
                char this_profile[100] = {};
                char id_locate[100] = {};
                filePointer9 = fopen(user_location, "r");
                if ( filePointer9 == NULL ){
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread, 50, filePointer9) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(this_profile, temporar);
                    }
                    fclose(filePointer9);
                }
                this_profile[strcspn(this_profile, "\n")] = 0;
                //printf("content of you is %s\n", this_profile);
                char thisss[200] = {};
                strcpy(thisss, this_profile);
                char* splitor3 = (char*) calloc(30, sizeof(char));
                splitor3 = strtok(this_profile, "{}\"[],:");
                int cur = 0;
                while(splitor3 != NULL){
                    if(strncmp("password", splitor3, 8) == 0){
                        cur = 1;
                    }
                    else if(cur == 1){
                        cur = 0;
                        strcpy(real_current_pass, splitor3);
                    }
                    splitor3 = strtok(NULL, "{}\"[],:");
                }
                //printf("real password is %s\n", real_current_pass);
                int correct_password = 0;
                if(strncmp(entered_current_pass, real_current_pass, maxx(strlen(entered_current_pass), strlen(real_current_pass))) == 0){
                    //printf("correct password\n");
                    correct_password = 1;
                }
                if(correct_password == 0){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"Entered current password is wrong.\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    ///4.(?)check if new pass is equal to current pass
                    int equality = 0;
                    entered_new_pass[strcspn(entered_new_pass,"\n")] = 0;
                    entered_current_pass[strcspn(entered_current_pass,"\n")] = 0;
                    if(strncmp(entered_current_pass, entered_new_pass, maxx(strlen(entered_new_pass), strlen(entered_current_pass))) == 0){
                        equality = 1;
                        //printf("they are equal\n");
                    }
                    if(equality == 1){
                        strcpy(response, "{\"type\":\"Error\",\"message\":\"New password is equal to current password.\"}");
                        printf("response: %s\n", response);
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                    else{
                        ///5.(?)change password in database
                        printf("I should change your password!!\n");
                        char search_current_pass[100] = {};
                        strcpy(search_current_pass, "\"password\":\"");
                        strcat(search_current_pass, entered_current_pass);
                        strcat(search_current_pass, "\"");
                        search_current_pass[strcspn(search_current_pass, "\n")] = 0;
                        char replace_password[100] = {};
                        strcpy(replace_password, "\"password\":\"");
                        strcat(replace_password, entered_new_pass);
                        strcat(replace_password, "\"");
                        replace_password[strcspn(replace_password, "\n")] = 0;
                        char* result = NULL;
                        printf("this profile before change is %s\n", thisss);
                        result = replaceWord1(thisss, search_current_pass, replace_password);
                        char new_profile[200] = {};
                        strcpy(new_profile, result);
                        printf("after %s\n", new_profile);
                        free(result);
                        FILE *filePointer27;
                        filePointer27 = fopen(user_location, "w");
                        if ( filePointer27 == NULL ){
                            printf( "file failed to open." );
                        }
                        else {
                            if (strlen(new_profile) > 0) {
                                fputs(new_profile, filePointer27);
                                fputs("\n", filePointer27);
                            }
                            fclose(filePointer27);
                        }
                        strcpy(response, "{\"type\":\"Successful\",\"message\":\"Password is changed successfully.\"}");
                        printf("response: %s\n", response);
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }

                }
            }
        }
//////////////////////////////////////////////////////follow/////////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "follow", 6) == 0){
            printf("****************************************\n");
            printf("Attempting to follow\n");
            ///
            ///get token->search for token->split the username ->search for the new user to be followed->follow->send response
            ///1.split the token and new username
            ///2.search for the token if it is acceptable and split the username
            ///3.(?) check the profile content and check if we have followed it before
            ///4.(?)follow it(change user file and that user's file)
            ///5.send response
            ///
            char token[100] ={};
            char token_and_user[100] = {};
            char user[100] = {};
            char new_user[100] = {};
            char profile_content[200] = {};
            char new_user_profile[200] = {};
            ///1
            char* splitor1 = (char*) calloc(100, sizeof(char));
            splitor1 = strtok(buffer, "\" ,:");
            int token_flag = 0;
            int new_user_flag = 0;
            while (splitor1 != NULL) {
                if(strncmp(splitor1, "follow", 6) == 0){
                    token_flag = 1;
                }
                else if(token_flag == 1){
                    token_flag = 0;
                    new_user_flag = 1;
                    strcpy(token, splitor1);
                }
                else if(new_user_flag == 1){
                    new_user_flag = 0;
                    strcpy(new_user, splitor1);
                }
                splitor1 = strtok(NULL, "\" ,:");
            }
            token[strcspn(token, "\n")] = 0;
            new_user[strcspn(new_user, "\n")] = 0;
            printf("Your sent token is %s\n", token);
            printf("new user to follow is %s\n", new_user);
            ///2
            int find_token = 0;
            for(int c = 0; c < tokk; c++){
                char temp1[100] = {};
                strcpy(temp1, token1[c]);
                if(strstr(temp1, token) != NULL){
                    find_token = 1;
                    strcpy(token_and_user, temp1);
                }
            }
            if(find_token == 0){
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                printf("your username and token are %s\n", token_and_user);
                char* splitor2 = (char*) calloc(30, sizeof(char));
                splitor2 = strtok(token_and_user, ",");
                int c_2 = 0;
                while(splitor2 != NULL){
                    if(c_2 == 1){
                        strcpy(user, splitor2);
                    }
                    splitor2 = strtok(NULL, ",");
                    c_2++;
                }
                printf("your username is %s\n", user);
                ///3
                char new_user_location[100] = {};
                strcpy(new_user_location, "./Resources/users/");
                strcat(new_user_location, new_user);
                strcat(new_user_location, ".txt");
                printf("location you are searching for is %s\n", new_user_location);
                FILE *filePointer44;
                char dataread[200];
                char temporar[200] = {};
                int that_user_exist = 1;
                filePointer44 = fopen(new_user_location, "r");
                if ( filePointer44 == NULL ){
                    printf( "file failed to open.\n" );
                    that_user_exist = 0;
                }
                else{
                    while( fgets ( dataread, 50, filePointer44) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(new_user_profile, temporar);
                    }
                    fclose(filePointer44) ;
                }
                printf("new user profile content is %s\n", new_user_profile);
                int followed_before = 0;
                char search_if_followed_before[100] = {};
                strcpy(search_if_followed_before, "\"");
                strcat(search_if_followed_before, new_user);
                strcat(search_if_followed_before, "\"");
                char my_locate1[100] = {};
                char profile_content11[100] = {};
                char temporar111[100] = {};
                char dataread111[100] = {};
                strcpy(my_locate1, "./Resources/users/");
                strcat(my_locate1, user);
                strcat(my_locate1, ".txt");
                printf("my location is %s\n", my_locate1);
                FILE *filePointer4415;
                filePointer4415 = fopen(my_locate1, "r");
                if ( filePointer4415 == NULL ){
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread111, 50, filePointer4415) != NULL ){
                        sprintf(temporar111, "%s", dataread111);
                        strcat(profile_content11, temporar111);
                    }
                    fclose(filePointer4415);
                }
                char after_followings[100] = {};
                sprintf(after_followings, "%s", strstr(profile_content11, "\"followings\":"));
                printf("after %s\n", after_followings);
                if(strstr(after_followings, search_if_followed_before) != NULL){
                    followed_before = 1;
                }
                if(followed_before == 1){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"User is already followed by you.\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    char* result = NULL;
                    char before[100] = {};
                    strcpy(before, "\"followers\":[");
                    char after[100] = {};
                    strcpy(after, before);
                    strcat(after, "\"");
                    strcat(after, user);
                    strcat(after, "\",");
                    result = replaceWord(new_user_profile, before, after);
                    printf("result is %s\n", result);
                    //free(result);
                    FILE *filePointer445;
                    char dataread22[200];
                    char temporar22[200] = {};
                    char my_locate[100] = {};
                    strcpy(my_locate, "./Resources/users/");
                    strcat(my_locate, user);
                    strcat(my_locate, ".txt");
                    printf("my location is %s\n", my_locate);
                    filePointer445 = fopen(my_locate, "r");
                    if ( filePointer445 == NULL ){
                        printf( "file failed to open.\n" );
                    }
                    else{
                        while( fgets ( dataread22, 50, filePointer445) != NULL ){
                            sprintf(temporar22, "%s", dataread22);
                            strcat(profile_content, temporar22);
                        }
                        fclose(filePointer445);
                    }
                    printf("my content is %s\n", profile_content);
                    char* result1 = NULL;
                    char before1[100] = {};
                    strcpy(before1, "followings\":[");
                    char after1[100] = {};
                    strcpy(after1, before1);
                    strcat(after1, "\"");
                    strcat(after1, new_user);
                    strcat(after1, "\",");
                    result1 = replaceWord(profile_content, before1, after1);
                    printf("result1 is %s\n", result1);
                    FILE *filePointer27;
                    filePointer27 = fopen(my_locate, "w");
                    if ( filePointer27 == NULL ){
                        printf( "file failed to open." );
                    }
                    else {
                        if (strlen(result1) > 0) {
                            fputs(result1, filePointer27);
                            fputs("\n", filePointer27);
                        }
                        fclose(filePointer27);
                    }

                    FILE *filePointer247;
                    filePointer247 = fopen(new_user_location, "w");
                    if ( filePointer247 == NULL ){
                        printf( "file failed to open." );
                    }
                    else {
                        if (strlen(result) > 0) {
                            fputs(result, filePointer247);
                            fputs("\n", filePointer247);
                        }
                        fclose(filePointer247);
                    }
                    strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            }
        }
///////////////////////////////////////////////////////unfollow//////////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "unfollow", 8) == 0){
            printf("****************************************\n");
            printf("Attempting to unfollow\n");
            ///
            ///get token->search for token->split the username ->search for the new user to be followed->follow->send response
            ///1.split the token and new username
            ///2.search for the token if it is acceptable and split the username
            ///3.(?) check the profile content and check if we have unfollowed it before
            ///4.(?)unfollow it(change user file and that user's file)
            ///5.send response
            ///
            char token[100] ={};
            char token_and_user[100] = {};
            char user[100] = {};
            char new_user[100] = {};
            char profile_content[200] = {};
            char new_user_profile[200] = {};
            ///1
            char* splitor1 = (char*) calloc(100, sizeof(char));
            splitor1 = strtok(buffer, "\" ,:");
            int token_flag = 0;
            int new_user_flag = 0;
            while (splitor1 != NULL) {
                if(strncmp(splitor1, "unfollow", 6) == 0){
                    token_flag = 1;
                }
                else if(token_flag == 1){
                    token_flag = 0;
                    new_user_flag = 1;
                    strcpy(token, splitor1);
                }
                else if(new_user_flag == 1){
                    new_user_flag = 0;
                    strcpy(new_user, splitor1);
                }
                splitor1 = strtok(NULL, "\" ,:");
            }
            token[strcspn(token, "\n")] = 0;
            new_user[strcspn(new_user, "\n")] = 0;
            printf("Your sent token is %s\n", token);
            printf("new user to unfollow is %s\n", new_user);
            ///2
            int find_token = 0;
            for(int c = 0; c < tokk; c++){
                char temp1[100] = {};
                strcpy(temp1, token1[c]);
                if(strstr(temp1, token) != NULL){
                    find_token = 1;
                    strcpy(token_and_user, temp1);
                }
            }
            if(find_token == 0){
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                printf("your username and token are %s\n", token_and_user);
                char* splitor2 = (char*) calloc(30, sizeof(char));
                splitor2 = strtok(token_and_user, ",");
                int c_2 = 0;
                while(splitor2 != NULL){
                    if(c_2 == 1){
                        strcpy(user, splitor2);
                    }
                    splitor2 = strtok(NULL, ",");
                    c_2++;
                }
                printf("your username is %s\n", user);
                ///3
                char new_user_location[100] = {};
                strcpy(new_user_location, "./Resources/users/");
                strcat(new_user_location, new_user);
                strcat(new_user_location, ".txt");
                printf("location you are searching for is %s\n", new_user_location);
                FILE *filePointer44;
                char dataread[200];
                char temporar[200] = {};
                int that_user_exist = 1;
                filePointer44 = fopen(new_user_location, "r");
                if ( filePointer44 == NULL ){
                    printf( "file failed to open.\n" );
                    that_user_exist = 0;
                }
                else{
                    while( fgets ( dataread, 50, filePointer44) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(new_user_profile, temporar);
                    }
                    fclose(filePointer44) ;
                }
                printf("new user profile content is %s\n", new_user_profile);
                int unfollowed_before = 0;
                char search_if_unfollowed_before[100] = {};
                strcpy(search_if_unfollowed_before, "\"");
                strcat(search_if_unfollowed_before, new_user);
                strcat(search_if_unfollowed_before, "\"");
                char my_locate1[100] = {};
                char profile_content11[100] = {};
                char temporar111[100] = {};
                char dataread111[100] = {};
                strcpy(my_locate1, "./Resources/users/");
                strcat(my_locate1, user);
                strcat(my_locate1, ".txt");
                printf("my location is %s\n", my_locate1);
                FILE *filePointer4415;
                filePointer4415 = fopen(my_locate1, "r");
                if ( filePointer4415 == NULL ){
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread111, 50, filePointer4415) != NULL ){
                        sprintf(temporar111, "%s", dataread111);
                        strcat(profile_content11, temporar111);
                    }
                    fclose(filePointer4415);
                }
                char after_followings[100] = {};
                sprintf(after_followings, "%s", strstr(profile_content11, "\"followings\":"));
                printf("after %s\n", after_followings);
                if(strstr(after_followings, search_if_unfollowed_before) != NULL){
                    unfollowed_before = 1;
                }
                if(unfollowed_before == 0){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"User is already unfollowed by you.\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    char result_up_to_now[100] = {};
                    char* result = NULL;
                    char before[100] = {};
                    strcpy(before, "\"followers\":[\"");
                    strcat(before, user);
                    strcat(before, "\"]");
                    char after[100] = {};
                    strcpy(after,"\"followers\":[]");
                    result = replaceWord(new_user_profile, before, after);
                    strcpy(result_up_to_now, result);
                    printf("result is %s\n", result_up_to_now);
                    //free(result);
                    char* result1 = NULL;
                    char before1[100] = {};
                    strcpy(before1, "\"followers\":[\"");
                    strcat(before1, user);
                    strcat(before1, "\",");
                    char after1[100] = {};
                    strcpy(after1,"\"followers\":[");
                    result1 = replaceWord(result_up_to_now, before1, after1);
                    strcpy(result_up_to_now, result1);
                    printf("result1 is %s\n", result_up_to_now);
                    char* result2 = NULL;
                    char before2[100] = {};
                    strcpy(before2, ",\"");
                    strcat(before2, user);
                    strcat(before2, "\"],\"followings");
                    char after2[100] = {};
                    strcpy(after2, "],\"followings");
                    result2 = replaceWord(result_up_to_now, before2, after2);
                    strcpy(result_up_to_now, result2);
                    printf("result2 is %s\n", result_up_to_now);
                    //free(result);
                    //free(result);
                    FILE *filePointer445;
                    char dataread22[200];
                    char temporar22[200] = {};
                    char my_locate[100] = {};
                    strcpy(my_locate, "./Resources/users/");
                    strcat(my_locate, user);
                    strcat(my_locate, ".txt");
                    printf("my location is %s\n", my_locate);
                    filePointer445 = fopen(my_locate, "r");
                    if ( filePointer445 == NULL ){
                        printf( "file failed to open.\n" );
                    }
                    else{
                        while( fgets ( dataread22, 50, filePointer445) != NULL ){
                            sprintf(temporar22, "%s", dataread22);
                            strcat(profile_content, temporar22);
                        }
                        fclose(filePointer445);
                    }
                    printf("my content is %s\n", profile_content);
                    char result_up_to_now11[100] = {};
                    char* result11 = NULL;
                    char before11[100] = {};
                    strcpy(before11, "followings\":[\"");
                    strcat(before11, new_user);
                    strcat(before11, "\"]");
                    char after11[100] = {};
                    strcpy(after11, "followings\":[]");
                    result11 = replaceWord(profile_content, before11, after11);
                    strcpy(result_up_to_now11, result11);
                    printf("result11 is %s\n", result_up_to_now11);
                    ///
                    char result_up_to_now22[100] = {};
                    char* result22 = NULL;
                    char before22[100] = {};
                    strcpy(before22, "followings\":[\"");
                    strcat(before22, new_user);
                    strcat(before22, "\",");
                    char after22[100] = {};
                    strcpy(after22, "followings\":[");
                    result22 = replaceWord(result_up_to_now11, before22, after22);
                    strcpy(result_up_to_now11, result22);
                    printf("result22 is %s\n", result_up_to_now11);
                    ///
                    char result_up_to_now33[100] = {};
                    char* result33 = NULL;
                    char before33[100] = {};
                    strcpy(before33, ",\"");
                    strcat(before33, new_user);
                    strcat(before33, "\"],\"personalTweets");
                    char after33[100] = {};
                    strcpy(after33, "],\"personalTweets");
                    result33 = replaceWord(result_up_to_now11, before33, after33);
                    strcpy(result_up_to_now11, result33);
                    printf("result33 is %s\n", result_up_to_now11);
                    ///
                    FILE *filePointer27;
                    filePointer27 = fopen(my_locate, "w");
                    if ( filePointer27 == NULL ){
                        printf( "file failed to open." );
                    }
                    else {
                        if (strlen(result_up_to_now11) > 0) {
                            fputs(result_up_to_now11, filePointer27);
                            fputs("\n", filePointer27);
                        }
                        fclose(filePointer27);
                    }

                    FILE *filePointer247;
                    filePointer247 = fopen(new_user_location, "w");
                    if ( filePointer247 == NULL ){
                        printf( "file failed to open." );
                    }
                    else {
                        if (strlen(result_up_to_now) > 0) {
                            fputs(result_up_to_now, filePointer247);
                            fputs("\n", filePointer247);
                        }
                        fclose(filePointer247);
                    }
                    strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            }
        }
///////////////////////////////////////////////////////////search/////////////////////////////////////////////////////////////////////////
        else if(strncmp("search", buffer, 5) == 0){
            printf("****************************************\n");
            printf("Attempting to search\n");
            ////
            char my_username[100] = {};
            char that_username[100] = {};
            char bio[100] = {};
            int numberOfFollowers;
            int numberOfFollowings;
            char followstatus[100] = {};
            char all_tweets[1000] = {};
            ////
            char my_token[100] = {};
            char my_token_and_user[100] = {};
            ////
            //printf("start\n");
            char* split6 = (char*) calloc(60, sizeof(char));
            buffer[strcspn(buffer, "\n")] = 0;
            split6 = strtok(buffer, ", ");
            int count = 0;
            while(split6 != NULL){
                //printf("split6 is %s\n", split6);
                if(count == 1){
                    strcpy(my_token, split6);
                }
                else if(count == 2){
                    strcpy(that_username, split6);
                }
                split6 = strtok(NULL, ", ");
                count++;
            }
            printf("token is %s\n", my_token);
            printf("that username is %s\n", that_username);
            int find_token = 0;
            for(int g = 0; g < tokk; g++){
                char helpy[100] = {};
                strcpy(helpy, token1[g]);
                if(strstr(helpy, my_token) != NULL){
                    strcpy(my_token_and_user, helpy);
                    find_token = 1;
                }
            }
            if(find_token == 0){
                ///do not have this token
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                ///token exists
                printf("token and user are %s\n", my_token_and_user);
                char* split7 = (char*) calloc(50, sizeof(char));
                split7 = strtok(my_token_and_user, ",");
                int count = 0;
                while(split7 != NULL){
                    if(count == 1){
                        strcpy(my_username, split7);
                    }
                    split7 = strtok(NULL, ",");
                    count++;
                }
                //printf("my username is %s\n", my_username);
                that_username[strcspn(that_username,"\n")] = 0;
                char search_for_this_username[100] = {};
                strcpy(search_for_this_username, "./Resources/users/");
                strcat(search_for_this_username, that_username);
                strcat(search_for_this_username, ".txt");
                char that_user_contents[200] = {};
                printf("that location is %s\n", search_for_this_username);
                FILE *filePointer44;
                char dataread[200];
                char temporar[200] = {};
                int that_user_exist = 1;
                filePointer44 = fopen(search_for_this_username, "r");
                if ( filePointer44 == NULL ){
                    printf( "file failed to open.\n" );
                    that_user_exist = 0;
                }
                else{
                    while( fgets ( dataread, 50, filePointer44) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(that_user_contents, temporar);
                    }
                    fclose(filePointer44) ;
                }
                if(that_user_exist == 0){
                    ///username does not exist
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is not valid.\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    that_user_contents[strcspn(that_user_contents, "\n")] = 0;
                    printf("that user content is %s\n", that_user_contents);
                    ///spliting the contents of this user and extracting the information
                    int bio_flag = 0;
                    int follower_flag = 0;
                    int follower_count = 0;
                    int following_flag = 0;
                    int following_count = 0;
                    int personal_tweets_flag = 0;
                    int all_tweets_count = 0;
                    int followed_before = 0;
                    char* profile_split = (char*) calloc(40, sizeof(char));
                    profile_split = strtok(that_user_contents, "\"{},:[]");
                    while(profile_split != NULL){
                        //printf("it is %s\n", profile_split);
                        if(strncmp("bio", profile_split, 3) == 0){
                            bio_flag = 1;
                        }
                        else if(bio_flag == 1 && strncmp("followers", profile_split, 9) == 0){
                            bio_flag = 0;
                            follower_flag = 1;
                        }
                        else if(bio_flag == 1 && strncmp("followers", profile_split, 9) != 0){
                            bio_flag = 0;
                            strcpy(bio, profile_split);
                        }
                        else if(strncmp("followers", profile_split, 9) == 0){
                            follower_flag = 1;
                        }
                        else if(follower_flag == 1 && strncmp("followings", profile_split, 10) != 0){
                            if(strncmp(profile_split, my_username, maxx(strlen(my_username), strlen(profile_split))) == 0){
                                followed_before = 1;
                            }
                            follower_count++;
                        }
                        else if(follower_flag == 1 && strncmp("followings", profile_split, 10) == 0){
                            follower_flag = 0;
                            following_flag = 1;
                        }
                        else if(strncmp("followings", profile_split, 10) == 0){
                            following_flag = 1;
                        }
                        else if(following_flag == 1 && strncmp("personalTweets", profile_split, 14) != 0){
                            following_count++;
                        }
                        else if(following_flag == 1 && strncmp("personalTweets", profile_split, 14) == 0){
                            following_flag = 0;
                            personal_tweets_flag = 1;
                        }
                        else if(strncmp("personalTweets", profile_split, 14) == 0){
                            personal_tweets_flag = 1;
                        }
                        else if(personal_tweets_flag == 1){
                            //printf("this is a personal tweet %s\n", profile_split);
                            FILE *filePointerp;
                            char dataread[200];
                            char temporar[200] = {};
                            char this_tweet[100] = {};
                            char id_locate[100] = {};
                            strcpy(id_locate, "./Resources/tweets/");
                            strcat(id_locate, profile_split);
                            strcat(id_locate, ".txt");
                            //printf("this id location is %s\n", id_locate);
                            filePointerp = fopen(id_locate, "r");
                            if ( filePointerp == NULL ){
                                printf( "file failed to open.\n" );
                            }
                            else{
                                while( fgets ( dataread, 50, filePointerp) != NULL ){
                                    sprintf(temporar, "%s", dataread);
                                    strcat(this_tweet, temporar);
                                }
                                fclose(filePointerp);
                            }
                            if(all_tweets_count != 0){
                                strcat(all_tweets, ",");
                            }
                            strcat(all_tweets, this_tweet);
                            all_tweets[strcspn(all_tweets, "\n")] = 0;
                            all_tweets_count++;
                            //printf("this tweet content is %s\n", this_tweet);
                        }
                        profile_split = strtok(NULL, "\"{},:[]");
                    }
                    if(followed_before == 1){
                        strcpy(followstatus, "Followed");
                    }
                    else{
                        strcpy(followstatus, "NotFollowed");
                    }
                    char num_follower[10]={};
                    sprintf(num_follower, "%d", follower_count);
                    char num_following[10]={};
                    sprintf(num_following, "%d", following_count);
                    //printf("number of followers is %s\n", num_follower);
                    //printf("number of followings is %s\n", num_following);
                    //printf("bio is %s\n", bio);
                    ////
                    strcpy(response, "{\"type\":\"Profile\",\"message\":{\"username\":\"");
                    strcat(response, that_username);
                    strcat(response, "\",\"bio\":\"");
                    strcat(response, bio);
                    strcat(response, "\",\"numberOfFollowers\":");
                    strcat(response, num_follower);
                    strcat(response, ",\"numberOfFollowings\":");
                    strcat(response, num_following);
                    strcat(response, ",\"followStatus\":\"");
                    strcat(response, followstatus);
                    strcat(response, "\",\"allTweets\":[");
                    strcat(response, all_tweets);
                    strcat(response, "]}}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            }
        }
/////////////////////////////////////////////////////comment/////////////////////////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "comment", 6) == 0){
            printf("****************************************\n");
            printf("Attempting to comment\n");
            ///
            ///1.split token, id to comment, comment
            ///2.validate token and split username
            ///3.(?)check if this id exists
            ///4.(?)comment the id
            ///5.send response
            ///
            char username[100] = {};
            char token[100] = {};
            char token_and_username[100] = {};
            char id[100] = {};
            char comment[100] = {};
            ///1
            char* split1 = (char*) calloc(30, sizeof(char));
            split1 = strtok(buffer, ", ");
            int c1 = 0;
            while(split1 != NULL){
                if(c1 == 1){
                    strcpy(token, split1);
                }
                else if(c1 == 2){
                    strcpy(id, split1);
                }
                else if(c1 == 3){
                    strcpy(comment, split1);
                }
                split1 = strtok(NULL, ", ");
                c1++;
            }
//            printf("token is %s\n", token);
//            printf("id is %s\n", id);
//            printf("comment is %s\n", comment);
            ///2
            int correct_token = 0;
            for(int c2 = 0; c2 < tokk; c2++){
                char temp2[100] = {};
                strcpy(temp2, token1[c2]);
                if(strstr(temp2, token) != NULL){
                    correct_token = 1;
                    strcpy(token_and_username, temp2);
                }
            }
            if(correct_token == 0){
                ///do not have this token
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                //printf("username and token are %s\n", token_and_username);
                char* split2 = (char*) calloc(30, sizeof(char));
                split2 = strtok(token_and_username, ",");
                int c22 = 0;
                while(split2 != NULL){
                    if(c22 == 1){
                        strcpy(username, split2);
                    }
                    split2 = strtok(NULL, ",");
                    c22++;
                }
                //printf("username is %s\n", username);
                ///3
                int id_exist = 1;
                FILE *filePointer0;
                char dataread0[200];
                char temporar0[200] = {};
                char this_tweet0[100] = {};
                char id_locate0[100] = {};
                strcpy(id_locate0, "./Resources/tweets/");
                strcat(id_locate0, id);
                strcat(id_locate0, ".txt");
                //printf("this id location is %s\n", id_locate);
                filePointer0 = fopen(id_locate0, "r");
                if ( filePointer0 == NULL ){
                    id_exist = 0;
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread0, 50, filePointer0) != NULL ){
                        sprintf(temporar0, "%s", dataread0);
                        strcat(this_tweet0, temporar0);
                    }
                    fclose(filePointer0);
                }
                if(id_exist == 0){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid id\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    printf("this tweet content is %s\n", this_tweet0);
                    char result_up_to_now[100] = {};
                    char* result = NULL;
                    char before[100] = {};
                    strcpy(before, "\"comments\":{");
                    char after[100] = {};
                    strcpy(after, before);
                    strcat(after, "\"");
                    strcat(after, username);
                    strcat(after, "\",\"");
                    comment[strcspn(comment, "\n")] = 0;
                    strcat(after, comment);
                    strcat(after, "\",");
                    result = replaceWord(this_tweet0, before, after);
                    strcpy(result_up_to_now, result);
                    //printf("result is %s\n", result_up_to_now);
                    char* result1 = NULL;
                    char before1[100] = {};
                    strcpy(before1, ",}");
                    char after1[100] = {};
                    strcpy(after1, "}");
                    result = replaceWord(result_up_to_now, before1, after1);
                    strcpy(result_up_to_now, result);
                    printf("result is %s\n", result_up_to_now);
                    FILE *filePointer27;
                    filePointer27 = fopen(id_locate0, "w");
                    if ( filePointer27 == NULL ){
                        printf( "file failed to open." );
                    }
                    else {
                        if (strlen(result_up_to_now) > 0) {
                            fputs(result_up_to_now, filePointer27);
                            fputs("\n", filePointer27);
                        }
                        fclose(filePointer27);
                    }
                    strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            }
        }
/////////////////////////////////////////////show comments//////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "show", 4) == 0){
            printf("****************************************\n");
            printf("Attempting to show\n");
            ///
            ///1.split token and id
            ///2.check if token is acceptable
            ///3.check if id is acceptable
            ///4.send response
            ///
            char token[100] = {};
            char id[100] = {};
            char id_content[100] = {};
            ///1
            char* split1 = (char*) calloc(30, sizeof(char));
            split1 = strtok(buffer, ", ");
            int c1 = 0;
            while(split1 != NULL){
                if(c1 == 1){
                    strcpy(token, split1);
                }
                if(c1 == 2){
                    strcpy(id, split1);
                }
                split1 = strtok(NULL, ", ");
                c1++;
            }
            id[strcspn(id, "\n")] = 0;
            token[strcspn(token, "\n")] = 0;
            printf("id is %s\n", id);
            printf("token is %s\n", token);
            ///2
            int correct_token = 0;
            for(int c2 = 0; c2 < tokk; c2++){
                char temp2[100] = {};
                strcpy(temp2, token1[c2]);
                if(strstr(temp2, token) != NULL){
                    correct_token = 1;
                }
            }
            if(correct_token == 0){
                ///do not have this token
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                ///3
                int id_exist = 1;
                FILE *filePointer;
                char dataread[200];
                char temporar[200] = {};
                char this_tweet[100] = {};
                char id_locate[100] = {};
                strcpy(id_locate, "./Resources/tweets/");
                strcat(id_locate, id);
                strcat(id_locate, ".txt");
                //printf("this id location is %s\n", id_locate);
                filePointer = fopen(id_locate, "r");
                if ( filePointer == NULL ){
                    id_exist = 0;
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread, 50, filePointer) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(this_tweet, temporar);
                    }
                    fclose(filePointer);
                }
                if(id_exist == 0){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid id\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    this_tweet[strcspn(this_tweet, "\n")] = 0;
                    strcpy(response, this_tweet);
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            }
        }
/////////////////////////////////////////////delete id//////////////////////////////////////////////////////////////
        else if(strncmp(buffer, "delete", 6) == 0){
            printf("****************************************\n");
            printf("Attempting to delete\n");
            char token[100] = {};
            char id[100] = {};
            ///1
            char* split1 = (char*) calloc(30, sizeof(char));
            split1 = strtok(buffer, ", ");
            int c1 = 0;
            while(split1 != NULL){
                if(c1 == 1){
                    strcpy(token, split1);
                }
                if(c1 == 2){
                    strcpy(id, split1);
                }
                split1 = strtok(NULL, ", ");
                c1++;
            }
            id[strcspn(id, "\n")] = 0;
            token[strcspn(token, "\n")] = 0;
            printf("id is %s\n", id);
            printf("token is %s\n", token);
            ///2
            int correct_token = 0;
            char every[100] = {};
            for(int c2 = 0; c2 < tokk; c2++){
                char temp2[100] = {};
                strcpy(temp2, token1[c2]);
                if(strstr(temp2, token) != NULL){
                    strcpy(every, temp2);
                    correct_token = 1;
                }
            }
            if(correct_token == 0){
                ///do not have this token
                strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid Token\"}");
                printf("response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
            else{
                char user11[100] = {};
                char* q = (char*) calloc(30, sizeof(char));
                q = strtok(every, ",");
                int h = 0;
                while(q != NULL){
                    if(h == 1){
                        strcpy(user11, q);
                    }
                    q = strtok(NULL, ",");
                    h++;
                }
                user11[strcspn(user11, "\n")] = 0;
                char id_locate[100] = {};
                int id_exist = 1;
                strcpy(id_locate, "./Resources/tweets/");
                strcat(id_locate, id);
                strcat(id_locate, ".txt");
                FILE *filePointer;
                char dataread[200];
                char temporar[200] = {};
                char this_tweet[100] = {};
                filePointer = fopen(id_locate, "r");
                if ( filePointer == NULL ){
                    id_exist = 0;
                    printf( "file failed to open.\n" );
                }
                else{
                    while( fgets ( dataread, 50, filePointer) != NULL ){
                        sprintf(temporar, "%s", dataread);
                        strcat(this_tweet, temporar);
                    }
                    fclose(filePointer);
                }
                //printf("this tweet is %s\n", this_tweet);
                if(id_exist == 0){
                    strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid id\"}");
                    printf("response: %s\n", response);
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
                else{
                    //
                    FILE *filePointer1;
                    char loca[100] = {};
                    strcpy(loca, "./Resources/users/");
                    strcat(loca, user11);
                    strcat(loca, ".txt");
                    char dataread1[200];
                    char temporar1[200] = {};
                    char this_tweet1[100] = {};
                    filePointer1 = fopen(loca, "r");
                    if ( filePointer1 == NULL ){
                        printf( "file failed to open.\n" );
                    }
                    else{
                        while( fgets ( dataread1, 50, filePointer1) != NULL ){
                            sprintf(temporar1, "%s", dataread1);
                            strcat(this_tweet1, temporar1);
                        }
                        fclose(filePointer1);
                    }
                    printf("content is %s\n", this_tweet1);
                    if(strstr(this_tweet1, id) == NULL){
                        strcpy(response, "{\"type\":\"Error\",\"message\":\"Invalid id because it is not yours\"}");
                        printf("response: %s\n", response);
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                    else{
                        char final[100] = {};
                        char* result = NULL;
                        result = replaceWord(this_tweet1, id, "");
                        strcpy(final, result);
                        char* result2 = replaceWord(final, ",]", "]");
                        strcpy(final, result2);
                        char* result3 = replaceWord(final, "[,", "[");
                        strcpy(final, result3);
                        printf("%s\n", final);
                        FILE *filePointer27;
                        filePointer27 = fopen(loca, "w");
                        if ( filePointer27 == NULL ){
                            printf( "file failed to open." );
                        }
                        else {
                            if (strlen(final) > 0) {
                                fputs(final, filePointer27);
                                fputs("\n", filePointer27);
                            }
                            fclose(filePointer27);
                        }
                        remove(id_locate);
                        strcpy(response, "{\"type\":\"Successful\",\"message\":\"\"}");
                        printf("response: %s\n", response);
                        send(client_socket, response, strlen(response), 0);
                        closesocket(client_socket);
                    }
                }
            }
        }
        send(client_socket, response, strlen(response), 0);
        closesocket(client_socket);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* generate_token(char * username){
    srand(time(NULL));
    char *string11 = (char*) calloc(50, sizeof(char));
    for(int i = 0; i < 25; ++i){
        string11[i] = '@' + rand()%26; // starting on '0', ending on '}'
    }
    return string11;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int maxx(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* replaceWord(const char* s, const char* oldW,const char* newW){
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s){
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else{
            result[i++] = *s++;
        }
    }
    result[i] = '\0';
    return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* replaceWord1(const char* s, const char* oldW,const char* newW){
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s){
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else{
            result[i++] = *s++;
        }
    }
    result[i] = '\0';
    return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////