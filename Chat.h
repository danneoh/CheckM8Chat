#ifndef CHAT_H
#define CHAT_H
#include "Chess.h"

/***** Function Declarations *****/
int main(int argc, char*argv[]);
void StartMenu();
void destroy(GtkWidget *widget, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void sendLoginRequest(GtkWidget *widget, gpointer user_data);
void sendRegisterRequest(GtkWidget *widget, gpointer user_data);
void sendFriendRequest(GtkWidget *widget, gpointer user_data);
void sendDeleteRequest(GtkWidget *widget, gpointer user_data);
void FatalError(const char *Program, const char *ErrorMsg);
void TranslateServerResponse(char ServerResponse[256]);
void Login();
void Register();
void ChatWindow();
void Dashboard();
void AddaFriend();
void AoDFriend();
void LogoutFunc();

#endif
