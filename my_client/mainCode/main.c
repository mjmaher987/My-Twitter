//Coded by Mohammad Javad Maheronnaghsh

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
//#include "slre.h"
#include <stdlib.h>
#include <windows.h>
#include <time.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initialize(int);
char *send_data(char *);
void print_new();
void clrscr();
void first_page();
//void clearScreen();
void menu();
void timeline();
void search();
void tweet_profile();
void personal_area();
void log_out();
void setbio();
void change_password();
void send_tweet();
void refresh();
void hidecursor();
void delay(int);
//void after_setbio();
void like_or_comment();
void show();
void delete();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int log_out1 = 0;
int er=0;
char logged_in_username[1000];
char token[1000];
struct sockaddr_in server;
int n_index = 0;
int qqq = 1;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    hidecursor();
    er = 0;
    //int port;
    //scanf("%d", &port);
    int can_initialize = initialize(12345);
    while (can_initialize == 0){
        first_page();
    }
//        else if(num == 1){
//            print_new();
//        }
    if(log_out1 == 0){
        clrscr();
        printf("DO you want to log out?\n");
        printf("1.Yes\n");
        printf("2.No\n");
        int s;
        //fflush(stdin);
        scanf("%d", &s);
        if( s == 1){
            log_out();
        }
        else{
            menu();
        }
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void first_page(){
    er = 0;
    clrscr();
    int num;
    printf("WELCOME TO TWITTER\n");
    printf("Please choose an option\n");
    printf("1.Sign Up\n");
    printf("2.Login\n");
    //fflush(stdin);
    scanf("%d", &num);
    //////////////////
    if (num == 2) {
        //Login
        er = 0;
        clrscr();
        printf("Username:\n");
        char message[1000] = "login ";
        char message1[1000] = {};
        scanf("%*c%[^\n]s", message1);
        strcpy(logged_in_username, message1);
        char message2[1000];
        printf("Password:\n");
        scanf("%*c%[^\n]s", message2);
        strcat(message, message1);
        strcat(message, ",");
        strcat(message, message2);
        strcat(message, "\n");
        char *feedback2 = send_data(message);
        //printf("%s\n\n", feedback2);
        //char b[10][1000] = {};
        char error[1000];
        char *pch = (char*) calloc(100, sizeof(char));
        pch = strtok(feedback2, "\",:.");
        int ii = 0;
        int flag_type = 0;
        int flag_token = 0;
        int flag_error = 0;
        int flag_real_time = 0;
        int pointer = 0;
        int count = 0;
        while (pch != NULL){
            if(strncmp("type", pch, 4) == 0){
                flag_type++;
                pointer = count;
            }
            else if(flag_type == 1 && count > pointer){
                flag_type = 0;
                if(strncmp(pch, "Token", 5) == 0){
                    flag_token++;
                }
                else if(strncmp(pch, "Error", 5) == 0){
                    flag_error = 1;
                }
                pointer = count;
            }
            else if(flag_error == 1 && count > pointer && strncmp("Incorrect Password", pch, 9) == 0){
                flag_error = 0;
                //clrscr();
                printf("Invlid password\n");
                delay(2);
                first_page();
            }
            else if(flag_error == 1 && count > pointer && strncmp("This username is not valid",pch , 24) == 0){
                flag_error = 0;
                //clrscr();
                printf("Invlid username\n");
                delay(2);
                first_page();
            }
            else if(flag_error == 1 && strncmp(pch, "This username is already signed in", 20)== 0){
                flag_error = 0;
                printf("This username is already logged in\n");
                delay(2);
                menu();
            }
            else if(flag_token == 1 && strncmp(pch, "message", 7) == 0){
                flag_token = 0;
                flag_real_time = 1;
                pointer = count;
            }
            else if(count > pointer && flag_real_time == 1){
                strcpy(token, pch);
                delay(2);
                menu();
            }
            pch = strtok(NULL, "\",:.");
            count++;
        }
    }
    //////////////////
    else if (num == 1) {
        //Signup
        er = 0;
        clrscr();
        printf("New Username:\n");
        char message1[1000];
        char message[1000] = "signup ";
        scanf("%*c%[^\n]s", message1);
        char message2[1000];
        printf("Password:\n");
        scanf("%*c%[^\n]s", message2);
        strcat(message, message1);
        strcat(message, ",");
        strcat(message, message2);
        strcat(message, "\n");
        char *feedback2 = send_data(message);
        char yy[100] = {};
        strcpy(yy, feedback2);
        //printf("%s\n\n", feedback2);
        char b1[10][1000] = {};
        //char error[1000];
        //char error_type[1000];
        char *pch1;
        pch1 = strtok(feedback2, "\":,.");
        int iii = 0;
        while (pch1 != NULL) {
            strcat(b1[iii], pch1);
            if (iii == 2) {
                if (strncmp(b1[2], "Error", 5) == 0) {
                    er++;
                }
            }
            //printf("%s\n", pch1);
            pch1 = strtok(NULL, "\",:.");
            iii++;
        }
        if (er == 0) {
            //clrscr();
            printf("Account Is Created");
            delay(2);
            first_page();
        }
        else {
            if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", yy, 50) == 0){
                printf("Invalid Token\n");
                delay(2);
                first_page();
            }
            else{
                printf("This username is already taken\n");
                delay(2);
                first_page();
            }
            //clrscr();
        }
    }
    else{
        return;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initialize(int port){
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(wsaStartUp != 0){
        printf("Error");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *send_data(char *data){
    char *buffer = malloc(1000);
    memset(buffer, 0, 1000);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET){
        return buffer;
    }
    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if (can_connect != 0){
        return buffer;
    }
    send(client_socket, data, strlen(data), 0);
    recv(client_socket, buffer, 999, 0);
    closesocket(client_socket);
    return buffer;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu(){
    while(TRUE){
        clrscr();
        printf("1.Timeline\n");
        printf("2.Search\n");
        printf("3.Tweet Profile\n");
        printf("4.Personal Area\n");
        printf("5.Delete Tweet\n");
        printf("6.Log Out\n");
        int a = 0;
        fflush(stdin);
        scanf("%d", &a);
        switch (a) {
            case 1:
                timeline();
                break;
            case 2:
                search();
                break;
            case 3:
                tweet_profile();
                break;
            case 4:
                personal_area();
                break;
            case 5:
                delete();
                break;
            case 6:
                log_out();
                break;
            default:
                menu();
                break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void timeline(){
    clrscr();
    printf("1.Send Tweet\n");
    printf("2.Refresh\n");
    printf("3.Back\n");
    int a = 0;
    fflush(stdin);
    //scanf("%*c");
    scanf("%d", &a);
    if(a==1){
        send_tweet();
    }
    else if(a==2){
        refresh();
    }
    else{
        menu();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void search(){
    clrscr();
    printf("Username:\n");
    char a[1000]="search ";
    char user[1000];
    //fflush(stdin);
    scanf("%s", user);
    strcat(a, token);
    strcat(a, ", ");
    strcat(a, user);
    strcat(a, "\n");
    char *feedback_search = send_data(a);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_search, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback_search);
        char b12[100][1000] = {};
        char *pch;
        char a6[1000];
        char *pch2;
        pch2 = strtok(feedback_search, "\":,.");
        int k = 0;
        int user_flag = 0;
        int bio_flag = 0;
        int content_flag = 0;
        int like_flag = 0;
        int comment_flag = 0;
        int follower_flag = 0;
        int following_flag = 0;
        int count = 0;
        int comment_count = 0;
        char username1[1000]={};
        //char idd[100]={};
        while(pch2 != NULL) {
            strcpy(a6, pch2);
            if(k == 2){
                //printf("%s", a6);
                if(strncmp(a6, "Error", 5) == 0){
                    printf("Not Found");
                    delay(2);
                    menu();
                }
            }
            if(strncmp(a6, "username", 8) == 0){
                user_flag++;
                count = k;
            }
            if(k > count && user_flag == 1){
                printf("username: %s\n", a6);
                strcpy(username1, a6);
                user_flag--;
            }
            if(strncmp(a6, "bio", 3) == 0){
                bio_flag++;
                count = k;
            }
            if(k > count && bio_flag == 1){
                if(strncmp(a6, "numberOfFollowers", 17) != 0){
                    printf("bio: %s\n", a6);
                }
                bio_flag--;
            }
            if(strncmp(a6, "content", 7) == 0){
                content_flag++;
                count = k;
            }
            if(content_flag == 1 && k> count){
                printf("\n%s\n", a6);
                content_flag--;
            }
            if(strncmp(a6, "numberOfFollowers", 17) == 0){
                follower_flag++;
                count = k;
            }
            if(follower_flag == 1 && k > count ){
                printf("followers: %s    ", a6);
                follower_flag--;
            }
            if(strncmp(a6, "numberOfFollowings", 18) == 0){
                following_flag++;
                count = k;
            }
            if(following_flag == 1 && k > count ){
                printf("followings: %s\n", a6);
                following_flag--;
            }
            if(strncmp(a6, "NotFollowed", 11) == 0){
                printf("follow\n");
            }
            if(strncmp(a6, "Followed", 8) == 0){
                printf("unfollow\n");
            }
            if(strncmp(a6, "likes", 5) == 0){
                like_flag++;
                count = k;
            }
            if(like_flag == 1 && k>count){
                printf("likes: %c\n", a6[0] );
                like_flag--;
            }
            if(strncmp(a6, "comments", 8) == 0){
                comment_flag++;
                count = k;
            }
            if(comment_flag == 1 && k-count > 0){
                if(strncmp(a6, "{}", 2) == 0){
                    printf("comments: %d   ", comment_count/2);
                    comment_flag--;
                    comment_count = 0;
                }
                else if(strncmp(a6, "}", 1) != 0 ){
                    comment_count++;
                }
                else{
                    printf("comments: %d   ", comment_count/2);
                    comment_flag--;
                    comment_count = 0;
                }
            }
            //printf("%s\n", pch2);
            pch2 = strtok(NULL, "\",:.");
            k++;
        }
        printf("\n1.Back\n");
        printf("2.Write follow or unfollow\n");
        qqq = 1;
        while(qqq == 1){
            char aaa[100]={};
            //fflush(stdin);
            scanf("%s", &aaa);
            if(aaa[0] == '1'){
                qqq--;
                menu();
            }
            else{
                strcat(aaa, " ");
                strcat(aaa, token);
                strcat(aaa, ", ");
                strcat(aaa, username1);
                strcat(aaa,"\n");
                char *feedback = send_data(aaa);
                //printf("%s\n", feedback);
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tweet_profile(){
    //fflush(stdin);
    clrscr();
    char tweet_pro11[1000]="profile ";
    strcat(tweet_pro11, token);
    strcat(tweet_pro11, "\n");
    char *feedback55 = send_data(tweet_pro11);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback55, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback55);
        char *pch12;
        pch12 = strtok(feedback55, "\",:[]{}");
        int ii3 = 0;
        int flag = 0;
        int my_id = 1;
        int my_username = 0;
        char my_user[100] = {};
        int my_bio = 0;
        char me_bio[100] = {};
        int my_follower = 0;
        char my_followers[100] = {};
        int my_following = 0;
        char my_followings[100] = {};
        // all_tweets = 0;
        int id_flag = 0;
        //int author_flag = 0;
        int content_flag = 0;
        int likes_flag = 0;
        int comments_flag = 0;
        int comment_count = 0;
        while (pch12 != NULL){
            if(my_id == 1 && strncmp("username", pch12, 8) == 0){
                my_username = 1;
            }
            else if(my_id == 1 && my_username == 1){
                strcpy(my_user, pch12);
                printf("Username: %s\n", my_user);
                my_username = 0;
            }
            else if(my_id == 1 && strncmp(pch12, "bio", 3) == 0){
                my_bio = 1;
            }
            else if(my_id == 1 && my_bio == 1 && strncmp(pch12, "numberOfFollowers", 16) != 0){
                strcpy(me_bio, pch12);
                printf("Bio: %s\n", me_bio);
                my_bio = 0;
            }
            else if(my_id == 1 && my_bio == 1 && strncmp(pch12, "numberOfFollowers", 16) == 0){
                my_bio = -1;
                my_follower = 1;
            }
            else if(my_id == 1 && strncmp(pch12, "numberOfFollowers", 16) == 0){
                my_follower = 1;
            }
            else if(my_id == 1 && my_follower == 1){
                strcpy(my_followers, pch12);
                printf("followers: %s    ", my_followers);
                my_follower = 0;
            }
            else if(my_id == 1 && strncmp(pch12, "numberOfFollowings", 17) == 0){
                my_following = 1;
            }
            else if(my_id == 1 && my_following == 1){
                strcpy(my_followings, pch12);
                printf("followings: %s\n\n", my_followings);
                my_following = 0;
                my_id = 0;
                my_username = 0;
                my_bio = 0;
                my_follower = 0;
            }
            else if(strncmp("id", pch12, 2) == 0){
                id_flag = 1;
            }
            else if(id_flag == 1){
                printf("id: %s\n", pch12);
                id_flag = 0;
            }
            else if(strncmp("content", pch12, 7) == 0){
                content_flag = 1;
            }
            else if(content_flag == 1){
                content_flag = 0;
                printf("%s\n", pch12);
            }
            else if(strncmp("comments", pch12, 8) == 0){
                comments_flag = 1;
                comment_count = 0;
            }
            else if(comments_flag == 1 && strncmp(pch12, "likes", 5) != 0){
                comment_count++;
            }
            else if(comments_flag == 1 && strncmp(pch12, "likes", 5) == 0){
                printf("comments: %d    ", comment_count/2);
                comments_flag = 0;
                likes_flag = 1;
            }
            else if(likes_flag == 1){
                printf("likes: %s\n\n", pch12);
                likes_flag = 0;
            }

            pch12 = strtok(NULL, "\",:[]{}");
            ii3++;
        }
        printf("1. Back\n");
        int back = 0;
        scanf("%d", &back);
        if(back == 1)
            return;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void personal_area(){
    clrscr();
    printf("1.Set Bio\n");
    printf("2.Change Password\n");
    printf("3.Back\n");
    int aq;
    //fflush(stdin);
    scanf("%d", &aq);
    if(aq==1){
        setbio();
    }
    else if(aq==2){
        change_password();
    }
    else if(aq == 3){
        menu();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void log_out(){
    clrscr();
    er = 0;
    char log_out[1000]="logout ";
    strcat(log_out, token);
    strcat(log_out, "\n");
    char *feedback3 = send_data(log_out);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback3, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback3);
        printf("Logged Out\n");
        log_out1++;
        delay(2);
        first_page();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setbio(){
    clrscr();
    char azz[1000]="set bio ";
    char bzz[1000];
    printf("Set a Profile\n");
    //fflush(stdin);
    scanf("%s", bzz);
    strcat(azz, token);
    strcat(azz, ", ");
    strcat(azz, bzz);
    strcat(azz, "\n");
    char *feedback_setbio = send_data(azz);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_setbio, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback_setbio);
        char a61[100];
//    char error[1000];
//    char error_type[1000];
        char *pch2;
        pch2 = strtok(feedback_setbio, "\":,.");
        int i4 = 0;
        while(pch2 != NULL) {
            if(i4 == 2){
                strcpy(a61, pch2);
            }
//        printf("%s\n", pch1);
            pch2 = strtok(NULL, "\",:.");
            i4++;
        }
        if(strncmp(a61, "S", 1) == 0){
            //clrscr();
            printf("Bio Is Updated\n");
        }
        else{
            //clrscr();
            printf("Error! Bio Is Already Updated\n");
        }
        delay(2);
        //after_setbio();
        clrscr();
        printf("1.Back\n");
        printf("2.Log Out\n");
        int bb54;
        //fflush(stdin);
        scanf("%d", &bb54);
        if(bb54 == 1){
            personal_area();
        }
        else{
            log_out();
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void change_password(){
    clrscr();
    char current11[100];
    char new11[100];
    printf("Current Password\n");
    //fflush(stdin);
    scanf("%s", current11);
    printf("New Password\n");
    scanf("%s", new11);
    char change[100]="change password ";
    strcat(change, token);
    strcat(change, ", ");
    strcat(change, current11);
    strcat(change, ", ");
    strcat(change, new11);
    strcat(change, "\n");
    char *feedbackk = send_data(change);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedbackk, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedbackk);

        char a6[100];
//    char error[1000];
//    char error_type[1000];
        char *pch2;
        pch2 = strtok(feedbackk, "\":,.");
        int i6 = 0;
        while(pch2 != NULL) {
            //printf("%s\n", pch2);
            if(i6 == 2){
                strcpy(a6, pch2);
                if(strncmp(a6, "Error", 5) == 0){
                    //clrscr();
                    printf("Error Changing Password\n");
                    delay(4);
                    personal_area();
                }
                else{
                    //clrscr();
                    printf("Password Changed\n");
                    delay(4);
                    personal_area();
                }
            }
            pch2 = strtok(NULL, "\",:.");
            i6++;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void clearScreen()
//{
//    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
//    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_tweet(){
    clrscr();
    printf("Write Tweet\n");
    char tweet1[1000];
    //f(stdin);
    scanf("%*c");
    gets(tweet1);
    char tweet[1000]="send tweet ";
    strcat(tweet, token);
    strcat(tweet, ", ");
    strcat(tweet, tweet1);
    strcat(tweet, "\n");
    //printf("%s", tweet);
    char *feedback3 = send_data(tweet);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback3, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback3);
        //clrscr();
        printf("Tweet Was Sent\n");
        //fflush(stdin);
        delay(2);
        timeline();
//    clrscr();
//    scanf("Please choose an option\n");
//    printf("1. Back\n");
//    printf("2. menu\n");
//    int c;
//    scanf("%d", &c);
//    if(c == 1){
//        timeline();
//    }
//    else if(c == 2){
//        menu();
//    }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void refresh(){
    clrscr();
    char ref[1000]="refresh ";
    strcat(ref, token);
    strcat(ref,"\n");
    char *feedback_ref = send_data(ref);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_ref, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback_ref);
        char a6[1000];
//    char error[1000];
//    char error_type[1000];
        char *pch2;
        pch2 = strtok(feedback_ref, "\":,.");
        int k = 0;
        int id_flag = 0;
        int author_flag = 0;
        int content_flag = 0;
        int like_flag = 0;
        int comment_flag = 0;
        int count = 1000;
        int comment_count = 0;
        char idd[100]={};
        int tweets = 0;
        while(pch2 != NULL) {
            strcpy(a6, pch2);
            if(strncmp(a6, "id", 2) == 0){
                id_flag++;
                count = k;
            }
            if(k > count && id_flag == 1){
                //printf("\nid: %s   ", a6);
                strcpy(idd, a6);
                id_flag--;
            }
            if(strncmp(a6, "author", 6) == 0){
                author_flag++;
                count = k;
            }
            if(k > count && author_flag == 1){
                printf("\n%s %s\n", a6, idd);
                author_flag--;
            }
            if(strncmp(a6, "content", 7) == 0){
                content_flag++;
                count = k;
            }
            if(content_flag == 1 && k> count){
                printf("%s\n", a6);
                content_flag--;
                tweets++;
            }
            if(strncmp(a6, "likes", 5) == 0){
                like_flag++;
                count = k;
            }
            if(like_flag == 1 && k>count){
                printf("likes: %c\n", a6[0] );
                like_flag--;
            }
            if(strncmp(a6, "comments", 8) == 0){
                comment_flag++;
                count = k;
            }
            if(comment_flag == 1 && k-count > 0){
                if(strncmp(a6, "{}", 2) == 0){
                    printf("comments: %d   ", comment_count/2);
                    comment_flag--;
                    comment_count = 0;
                }
                else if(strncmp(a6, "}", 1) != 0 ){
                    comment_count++;
                }
                else{
                    printf("comments: %d   ", comment_count/2);
                    comment_flag--;
                    comment_count = 0;
                }
            }
            //printf("%s\n", pch2);
            pch2 = strtok(NULL, "\",:.");
            k++;
        }

        printf("\n\n1.Back\n");
        if(tweets != 0){
            printf("2.Like Or Comment A Tweet\n");
            printf("3.Show comments\n");
        }
        int aaa;
        //fflush(stdin);
        scanf("%d", &aaa);
        if(aaa == 2){
            like_or_comment();
        }
        else if(aaa == 3){
            show();
        }
        else{
            timeline();
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clrscr(){
    system("@cls||clear");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hidecursor(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delay(int second){
    int milsec = 1000 * second;
    clock_t startTime = clock();
    while(clock() < (startTime + milsec));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void after_setbio(){
//    clrscr();
//    printf("Please choose an option\n");
//    printf("1.Back");
//    printf("2. Log out");
//    int bb;
//    scanf("%d", &bb);
//    if(bb == 1){
//        menu();
//    }
//    else if(bb == 2){
//        log_out();
//    }
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void like_or_comment(){
    clrscr();
    printf("1.Like\n");
    printf("2.Comment\n");
    printf("3.Back\n");
    int j;
    //fflush(stdin);
    scanf("%d", &j);
    if(j == 1){
        clrscr();
        char a[1000]={};
        char b[1000]="like ";
        clrscr();
        printf("Enter an Id to Like\n");
        //fflush(stdin);
        scanf("%s", a);
        strcat(b, token);
        strcat(b, ", ");
        strcat(b, a);
        strcat(b,"\n");
        char *feedback_like = send_data(b);
        if(strncmp(feedback_like, "{\"type\":\"Error\",\"message\":\"You have liked this before.\"}", 64) == 0){
            printf("you have liked it before\n");
            delay(2);
            like_or_comment();
        }
        else if(strncmp(feedback_like, "{\"type\":\"Successful\",\"message\":\"\"}", 42) == 0){
            printf("tweet was liked\n");
            delay(2);
            like_or_comment();
        }
        else if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_like, 50) == 0){
            printf("Invalid Token\n");
            delay(2);
            first_page();
        }
    }
    else if(j == 2){
        clrscr();
        char a[1000]={};
        char c[1000]={};
        char b[1000]="comment ";
        clrscr();
        printf("Enter an Id to Comment\n");
        //fflush(stdin);
        scanf("%s", a);
        printf("Enter a Comment\n");
        //fflush(stdin);
        scanf("%s", c);
        strcat(b, token);
        strcat(b, ", ");
        strcat(b, a);
        strcat(b, ", ");
        strcat(b, c);
        strcat(b,"\n");
        char *feedback_comment = send_data(b);
        if(strncmp(feedback_comment, "{\"type\":\"Error\",\"message\":\"Invalid id\"}", 47) == 0){
            printf("this id does not exist\n");
            delay(2);
            like_or_comment();
        }
        else if(strncmp(feedback_comment, "{\"type\":\"Successful\",\"message\":\"\"}", 42) == 0){
            printf("comment sent\n");
            delay(2);
            like_or_comment();
        }
        else if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_comment, 50) == 0){
            printf("Invalid Token\n");
            delay(2);
            first_page();
        }
        //printf("%s\n", feedback_comment);
    }
    else if(j == 3){
        timeline();
    }
    else{
        like_or_comment();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show(){
    clrscr();
    printf("Enter an id to show comments\n");
    char a[100] = {};
    char b[100] = "show ";
    scanf("%s", a);
    strcat(b, token);
    strcat(b, ", ");
    strcat(b, a);
    strcat(b, "\n");
    char *feedback_show = send_data(b);
    if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_show, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    else{
        //printf("%s\n", feedback_show);
        char* split = (char*) calloc(30, sizeof(char));
        split = strtok(feedback_show, ",:\"{}[]");
        int comment = 0;
        int author = 1;
        while(split != NULL){
            if(strncmp(split, "comment", 7) == 0){
                comment = 1;
            }
            else if(strncmp(split, "likes", 5) == 0){
                comment = 0;
            }
            else if(comment == 1 && author == 1){
                printf("author: %s***", split);
                author = 0;
            }
            else if(comment == 1 && author == 0){
                printf("content: %s\n", split);
                author = 1;
            }
            split = strtok(NULL, ",:\"{}[]");
        }
        delay(3);
        timeline();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delete(){
    clrscr();
    printf("Enter an id to delete\n");
    char a[100] = {};
    char b[100] = "delete ";
    scanf("%s", a);
    strcat(b, token);
    strcat(b, ", ");
    strcat(b, a);
    strcat(b, "\n");
    char *feedback_delet = send_data(b);
    if(strncmp(feedback_delet, "{\"type\":\"Error\",\"message\":\"Invalid id\"}", 47) == 0){
        printf("this id does not exist\n");
        delay(3);
        return;
    }
    else if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid id because it is not yours\"}", feedback_delet, 71) == 0){
        printf("this tweet is not yours\n");
        delay(3);
        return;
    }
    else if(strncmp("{\"type\":\"Successful\",\"message\":\"\"}", feedback_delet, 42) == 0){
        printf("delted successfully\n");
        delay(3);
        return;
    }
    else if(strncmp("{\"type\":\"Error\",\"message\":\"Invalid Token\"}", feedback_delet, 50) == 0){
        printf("Invalid Token\n");
        delay(2);
        first_page();
    }
    //printf("%s\n", feedback_show);
    //scanf("%*c");

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
