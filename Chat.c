#include <gtk/gtk.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Chat.h"
//#include "Chess.h"

/***** Global variables used in most functions *****/
int l, n;
int SocketFD,	/* socket file descriptor */
    PortNo;		/* port number */
struct sockaddr_in
    ServerAddress;	/* server address we connect with */
struct hostent
    *Server;	/* server host */
char SendBuf[256];	/* message buffer for sending a message */
char RecvBuf[256];	/* message buffer for receiving a response */

GtkWidget *usernameentry, *passwordentry;
GtkWidget *usernameregentry, *passwordregentry;
GtkWidget *AddUsernameentry, *DeleteUserentry;

const gchar *user_entry_text;
const gchar *pass_entry_text;

const gchar *userreg_entry_text;
const gchar *passreg_entry_text;

const gchar *Add_entry_text;
const gchar *Del_entry_text;

char *program;

/***** Function Definitions *****/
void talk2Server(){

        l = strlen(SendBuf);
        if (SendBuf[l-1] == '\n')
        {   SendBuf[--l] = 0;
        }
        if (l)
        {   printf("%s: Sending command '%s'...\n", program, SendBuf);
            n = write(SocketFD, SendBuf, l);
            if (n < 0)
            {   FatalError(program, "writing to socket failed");
            }
#ifdef DEBUG
            printf("%s: Waiting for response...\n", program);
#endif
             n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if (n < 0)
            {   FatalError(program, "reading from socket failed");
            }
            RecvBuf[n] = 0;
            printf("%s: Received response: %s\n", program, RecvBuf);
         TranslateServerResponse(RecvBuf);
       }


}

void sendServer(){

        l = strlen(SendBuf);
        if (SendBuf[l-1] == '\n')
        {   SendBuf[--l] = 0;
        }
        if (l)
        {   printf("%s: Sending command '%s'...\n", program, SendBuf);
            send(SocketFD, SendBuf, l, 0);
            if (n < 0)
            {   FatalError(program, "writing to socket failed");
            }
        }


}
void AorDFriend()
{
    GtkWidget *AoDWindow, *AoDLayout, *AoDbackground; 
    GtkWidget *AcceptFriend, *DeclineFriend;
    GtkWidget *Addlabel, *Declinelabel;  
    GtkWidget *Addimage, *Decimage;
 
    /*This makes the pop up window*/
    AoDWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(AoDWindow, 400, 300); 
    gtk_container_set_border_width(GTK_CONTAINER(AoDWindow), 10);
    gtk_window_set_position(GTK_WINDOW(AoDWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(AoDWindow), "Check-Mates's Chat Add a Friend");
    gtk_window_set_resizable(GTK_WINDOW(AoDWindow), FALSE);
   
    /*This is for when you press "X" of the window you close it*/
    g_signal_connect(AoDWindow, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(AoDWindow, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    AoDLayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(AoDWindow), AoDLayout);
    gtk_widget_show(AoDLayout);
   
    /*This is the images for background of the layout and buttons*/
    AoDbackground = gtk_image_new_from_file("./background/chatback.png");   
    Addimage = gtk_image_new_from_file("./background/lwood.png");
    Decimage = gtk_image_new_from_file("./background/lwood.png");
  
    /*Labels for buttons*/
    Addlabel = gtk_label_new("Add User");
    Declinelabel = gtk_label_new("Decline User");
   
    /*This is the background*/
    gtk_layout_put(GTK_LAYOUT(AoDLayout), AoDbackground, 0, 0);

    /*First button which just says Accept*/
    AcceptFriend = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(AcceptFriend, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(AcceptFriend, "clicked", G_CALLBACK(gtk_widget_hide), AoDWindow);
    g_signal_connect_swapped(AcceptFriend, "clicked", G_CALLBACK(Dashboard), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(AoDLayout), AcceptFriend, 90, 30);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(AoDLayout), Addlabel, 165, 65);
    gtk_widget_set_size_request(AcceptFriend, 215, 80);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(AcceptFriend), Addimage);//sets the image of the wood grain inside the button

    /*Second button which just says Accept*/
    DeclineFriend = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(DeclineFriend, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(DeclineFriend, "clicked", G_CALLBACK(gtk_widget_hide), AoDWindow);
    g_signal_connect_swapped(DeclineFriend, "clicked", G_CALLBACK(Dashboard), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(AoDLayout), DeclineFriend, 90, 170);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(AoDLayout), Declinelabel, 155, 190);
    gtk_widget_set_size_request(DeclineFriend, 215, 80);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(DeclineFriend), Decimage);//sets the image of the wood grain inside the button

    gtk_widget_show_all(AoDWindow); 
}
void recvServer(){

    int len;
    char inmsg[5];
    char friend[10];
    char msg[255];

    while(strcmp("BBYE", RecvBuf) != 0){
	    //RecvBuf[0] = 0;
            len = (int)recv(SocketFD, RecvBuf, 255, 0);
            if (len < 0)
            {   FatalError(program, "reading from socket failed");
            }
            RecvBuf[len] = 0;
            printf("%s: Received response: %s\n", program, RecvBuf);
            TranslateServerResponse(RecvBuf);
	    
	    strncpy(inmsg, RecvBuf, 5);
	    inmsg[4] = 0;

	    if(strcmp(inmsg, "RMSG") == 0){
		int i =10;
 		while(RecvBuf[i] != '&'){
			friend[i-10] = RecvBuf[i];
			i++;
		}
		
		i += 6;
		int c = 0;
		while(RecvBuf[i] != 0 || c != 254){
			msg[c] = RecvBuf[i];

			i++;
			c++;
		}

		g_print("%s: %s\n", friend, msg);


	    }else if(strcmp(inmsg, "FRIE") == 0){
		for(int i=5; RecvBuf[i] != 0; i++){
			friend[i-5] = RecvBuf[i];
		}
		g_print("Friend: %s\n", friend);
		AorDFriend();
	    }

	    sleep(1);
	    RecvBuf[0] = 0;
	    
         
         
    }
	g_print("Stopped Receiving!");

}

void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    strcpy(SendBuf, "BBYE");
    sendServer();
    sleep(1);
    g_print("Goodbye.");
    close(SocketFD);
    return FALSE;
}

void sendLoginRequest(GtkWidget *widget, gpointer user_data)
{
    strcpy(RecvBuf, "dick");
    char use[10];
    char pass[10];
    char command[30] = "LOGN?NAME=";

    RecvBuf[0] = 0;
     
    user_entry_text = gtk_entry_get_text(GTK_ENTRY(usernameentry));
    pass_entry_text = gtk_entry_get_text(GTK_ENTRY(passwordentry));
 
    g_print("username entry is: %s\n", user_entry_text);
    g_print("password entry is: %s\n", pass_entry_text);

    sprintf(use, "%s", user_entry_text);
    sprintf(pass, "%s", pass_entry_text);
    

    strcat(command, use);

    strcat(command, "&PASS=");
    strcat(command, pass);
 
    g_print("%s", command);

    strncpy(SendBuf, command, strlen(command));
    SendBuf[strlen(command)] = 0;

    sendServer();
    sleep(1);
     if(strcmp("INLN", RecvBuf) == 0 || strcmp("WPAS", RecvBuf) == 0)
	{
		Login();
	}
     else if(strcmp("OKLN", RecvBuf) == 0)
	{
		Dashboard();
	}


}//end sendLoginRequet
void LogoutFunc()
{
    
    strcpy(SendBuf, "BBYE");
    sendServer();
    sleep(1);
    if(strcmp("BBYE", RecvBuf) == 0)
    {
        StartMenu();
    }

}
void sendRegisterRequest(GtkWidget *widget, gpointer user_data)
{
    char usereg[10];
    char passreg[10];
    char commandreg[30] = "REGI?UNAM=";
    RecvBuf[0] = 0;

    userreg_entry_text = gtk_entry_get_text(GTK_ENTRY(usernameregentry));
    passreg_entry_text = gtk_entry_get_text(GTK_ENTRY(passwordregentry));

    g_print("username entry is: %s\n", userreg_entry_text);
    g_print("password entry is: %s\n", passreg_entry_text);

    sprintf(usereg, "%s", userreg_entry_text);
    sprintf(passreg, "%s", passreg_entry_text);


    strcat(commandreg, usereg);

    strcat(commandreg, "&PASS=");
    strcat(commandreg, passreg);

    g_print("%s\n", commandreg);

    strncpy(SendBuf, commandreg, strlen(commandreg));
    SendBuf[strlen(commandreg)] = 0; 
    sendServer();
    sleep(1);
    if(strcmp("INRG", RecvBuf) == 0 || strcmp("ALEX", RecvBuf) == 0)
    {
        Register();
    }
    else if(strcmp("OKRG", RecvBuf) == 0)
    {
        StartMenu();    
    }
}
void sendFriendRequest(GtkWidget *widget, gpointer data)
{
    char adduser[20];
    char commandadd[30] = "ADDF?NAME=";
    RecvBuf[0] = 0;

    Add_entry_text = gtk_entry_get_text(GTK_ENTRY(AddUsernameentry));
  
    g_print("Username entered is: %s\n", Add_entry_text);

    sprintf(adduser, "%s\n", Add_entry_text);

    strcat(commandadd, adduser);

    g_print("%s\n", commandadd);
 
    strncpy(SendBuf, commandadd, strlen(commandadd));
    SendBuf[strlen(commandadd)] = 0;
    sendServer();
    sleep(1);
    if(strcmp("NUSR", RecvBuf) == 0)
    {
        g_print("User Not Found\n");
        AddaFriend();
    }
    if(strcmp("NOLN", RecvBuf) == 0)
    {
        g_print("User Not Online\n");
        AddaFriend();
    }

    if(strcmp("ISFR", RecvBuf) == 0)
    {
        g_print("You two are already friends\n");
        AddaFriend();
    }
    else
    {
        AddaFriend();
    }

}
void sendDeleteRequest(GtkWidget *widget, gpointer user_data)
{
    char deluser[20];
    char delcommand[30] = "DELF?NAME=";
    RecvBuf[0] = 0; 

    Del_entry_text = gtk_entry_get_text(GTK_ENTRY(DeleteUserentry));

    g_print("Username entered is: %s\n", Del_entry_text);
 
    sprintf(deluser, "%s\n", Del_entry_text);

    strcat(delcommand, deluser);

    g_print("%s\n", delcommand);

    strncpy(SendBuf, delcommand, strlen(delcommand));
    SendBuf[strlen(delcommand)] = 0;
    sendServer();
    sleep(1);
    if(strcmp("DESU", RecvBuf) == 0)
    {
        g_print("User Deleted Successfully\n");
        AddaFriend();
    }
    else if(strcmp("NUSR", RecvBuf) == 0)
   {
        g_print("No Such User Exsist\n");
        AddaFriend();
   }

}
/*************************************************************************************/

/*****************************************************************************************/
void AddaFriend()
{
    GtkWidget *window, *AddFriendlayout, *AddFriendbackground;
    GtkWidget *backbutton, *Addbutton, *Addbackgroundbutton, *Deletebutton, *Deletebackgroundbutton;
    GtkWidget *backbuttonimage, *Addbuttonimage, *Addbackgroundbuttonimage, *Deletebuttonimage, *Deletebackgroundbuttonimage;
    GtkWidget *backlabel, *Addlabel, *AddUserlabel, *Deletelabel, *DeleteUserlabel;

    /*Initializes the window*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Check-Mates's Chat Add a Friend");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /*This is for when you press "X" of the window you close it*/
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    AddFriendlayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), AddFriendlayout);
    gtk_widget_show(AddFriendlayout);

    /*These are the labels for the buttons*/
    Addlabel = gtk_label_new("Enter User You Want\n           To Add");
    AddUserlabel = gtk_label_new("Add");
    Deletelabel = gtk_label_new("Enter User You Want\n         To Delete");
    DeleteUserlabel = gtk_label_new("Delete");
    backlabel = gtk_label_new("Back");

    /*This is the images for background of the layout and buttons*/
    AddFriendbackground = gtk_image_new_from_file("./background/chatback.png");
    Addbackgroundbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    Deletebackgroundbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    Addbuttonimage = gtk_image_new_from_file("./background/registerwood.png"); 
    Deletebuttonimage = gtk_image_new_from_file("./background/registerwood.png");      
    backbuttonimage = gtk_image_new_from_file("./background/backlwood.png");

     /*This is the background*/
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), AddFriendbackground, 0, 0);

    /*First button which just says Add Friend*/
    Addbackgroundbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Addbackgroundbutton, 300, 200);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Addlabel, 345, 210);//first number is x axis
    gtk_widget_set_size_request(Addbackgroundbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(Addbackgroundbutton), Addbackgroundbuttonimage);//sets the image of the wood grain inside the button
  
    /*Second button which takes user info and checks if it can add the user*/
    Addbutton = gtk_button_new_with_label(NULL);
    g_signal_connect(Addbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(Addbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(Addbutton, "clicked", G_CALLBACK(sendFriendRequest), NULL);
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Addbutton, 355, 300);//first number is X axis, second is Y axis
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), AddUserlabel, 385, 310);
    gtk_widget_set_size_request(Addbutton, 90, 35);//first number is width, second is height
    gtk_button_set_image(GTK_BUTTON(Addbutton), Addbuttonimage);
 
    /*Third button which just says Delete a Friend*/
    Deletebackgroundbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Deletebackgroundbutton, 300, 350);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Deletelabel, 345, 360);
    gtk_widget_set_size_request(Deletebackgroundbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(Deletebackgroundbutton), Deletebackgroundbuttonimage);//sets the image of the wood grain inside the button

    /*Fourth button which takes user info and checks if it can delete the user*/
    Deletebutton = gtk_button_new_with_label(NULL);
    g_signal_connect(Deletebutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(Deletebutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(Deletebutton, "clicked", G_CALLBACK(sendDeleteRequest), NULL);
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), Deletebutton, 355, 450);//first number is X axis, second is Y axis
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), DeleteUserlabel, 375, 460);
    gtk_widget_set_size_request(Deletebutton, 90, 35);//first number is width, second is height
    gtk_button_set_image(GTK_BUTTON(Deletebutton), Deletebuttonimage);

    /*Fifth button which takes the user back to Dashboard*/
    backbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(Dashboard), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), backbutton, 0, 0);//puts the button on the layout first number is the y axis
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), backlabel, 15, 5);//places label on top of button
    gtk_widget_set_size_request(backbutton, 70, 30);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(backbutton), backbuttonimage);//sets the image of the wood grain inside the button


    /*This creates a texbox for the user to enter their username*/
    AddUsernameentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(AddUsernameentry), 0);
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), AddUsernameentry, 330,245);
 
    DeleteUserentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(DeleteUserentry), 0);
    gtk_layout_put(GTK_LAYOUT(AddFriendlayout), DeleteUserentry, 330, 395);   
 
    gtk_widget_show_all(window);
   
}
/***************************************************************************************/
void ChatWindow()
{
    GtkWidget *WindowChat, *Chatlayout;
    GtkWidget *Chatbackground;
    GtkWidget *Textbox;
    GtkWidget *scrolled_window;   


    /*Initializes the window*/
    WindowChat = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(WindowChat, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(WindowChat), 10);
    gtk_window_set_position(GTK_WINDOW(WindowChat), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(WindowChat), "Check-Mates's Chat Window\n");
    gtk_window_set_resizable(GTK_WINDOW(WindowChat), FALSE);

    /*This is for when you press "X" of the window you close it*/
    g_signal_connect(WindowChat, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(WindowChat, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    Chatlayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(WindowChat), Chatlayout);
    gtk_widget_show(Chatlayout);

    /*This is the images for background of the layout and buttons*/
    Chatbackground = gtk_image_new_from_file("./background/chatback.png");   
 
    /*This is the background*/
    gtk_layout_put(GTK_LAYOUT(Chatlayout), Chatbackground, 0, 0);
    
    Textbox = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(Textbox), GTK_WRAP_WORD); 
    gtk_layout_put(GTK_LAYOUT(Chatlayout), Textbox, 0, 0);    

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC); 
     gtk_container_add (GTK_CONTAINER (scrolled_window), 
                                         Textbox);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 5);
 
  
  gtk_container_add (GTK_CONTAINER (WindowChat), scrolled_window);




 
    gtk_widget_show_all(WindowChat);
}

/***************************************************************************************/
void Dashboard()
{
    GtkWidget *Dashboardwindow;
    GtkWidget *Dashboardlayout;
    GtkWidget *Dashboardbackground;
    GtkWidget *logoutbutton, *Chessbutton, *Chatbutton, *AddFriendbutton;
    GtkWidget *logoutbuttonimage, *Chessbuttonimage, *Chatbuttonimage, *AddFriendbuttonimage;   
    GtkWidget *logoutlabel, *Chesslabel, *Chatlabel, *AddFriendlabel;
 
    /*Initializes the window*/
    Dashboardwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(Dashboardwindow, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(Dashboardwindow), 10);
    gtk_window_set_position(GTK_WINDOW(Dashboardwindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(Dashboardwindow), "Check-Mates's Chat Dashboard");
    gtk_window_set_resizable(GTK_WINDOW(Dashboardwindow), FALSE);
  
    /*This is for when you press "X" of the window, it closes it*/
    g_signal_connect(Dashboardwindow, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(Dashboardwindow, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    Dashboardlayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(Dashboardwindow), Dashboardlayout);
    gtk_widget_show(Dashboardlayout);

    /*These are the labels for the buttons*/
    Chatlabel = gtk_label_new("Chat");
    Chesslabel = gtk_label_new("Chess");
    AddFriendlabel = gtk_label_new("Add Friend");
    logoutlabel = gtk_label_new("Logout");

    /*This is the images for background of the layout and buttons*/
    Dashboardbackground = gtk_image_new_from_file("./background/chatback.png");
    Chessbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    Chatbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    AddFriendbuttonimage = gtk_image_new_from_file("./background/lwood.png");  
    logoutbuttonimage = gtk_image_new_from_file("./background/backlwood.png");

    /*This is the background*/
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), Dashboardbackground, 0, 0);

    /*First button which just says Chess*/
    Chessbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(Chessbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(Chessbutton, "clicked", G_CALLBACK(gtk_widget_hide), Dashboardwindow);
    g_signal_connect_swapped(Chessbutton, "clicked", G_CALLBACK(Chess), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), Chessbutton, 550, 125);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), Chesslabel, 645, 158);
    gtk_widget_set_size_request(Chessbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(Chessbutton), Chessbuttonimage);//sets the image of the wood grain inside the button
 
    /*Second button which just says Chat*/
    Chatbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(Chatbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(Chatbutton, "clicked", G_CALLBACK(gtk_widget_hide), Dashboardwindow);
    g_signal_connect_swapped(Chatbutton, "clicked", G_CALLBACK(ChatWindow), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), Chatbutton, 550, 245);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), Chatlabel, 640, 285);
    gtk_widget_set_size_request(Chatbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(Chatbutton), Chatbuttonimage);//sets the image of the wood grain inside the button

    /*Third button which just says AddFriend*/
    AddFriendbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(AddFriendbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(AddFriendbutton, "clicked", G_CALLBACK(gtk_widget_hide), Dashboardwindow);
    g_signal_connect_swapped(AddFriendbutton, "clicked", G_CALLBACK(AddaFriend), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), AddFriendbutton, 550, 365);//puts the button on the layout first number is the y axis
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), AddFriendlabel, 625, 400);//places label on top of button
    gtk_widget_set_size_request(AddFriendbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(AddFriendbutton), AddFriendbuttonimage);//sets the image of the wood grain inside the button
      
    /*Fourth button which takes the user back to Dashboard*/
    logoutbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(logoutbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(logoutbutton, "clicked", G_CALLBACK(gtk_widget_hide), Dashboardwindow);
    g_signal_connect_swapped(logoutbutton, "clicked", G_CALLBACK(LogoutFunc), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), logoutbutton, 0, 0);//puts the button on the layout first number is the y axis
    gtk_layout_put(GTK_LAYOUT(Dashboardlayout), logoutlabel, 10, 5);//places label on top of button
    gtk_widget_set_size_request(logoutbutton, 70, 30);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(logoutbutton), logoutbuttonimage);//sets the image of the wood grain inside the button
   


    gtk_widget_show_all(Dashboardwindow);
}
void Login()
{ 
    GtkWidget *window;
    GtkWidget *loginlayout;
    GtkWidget *backlabel, *loginlabel, *EnterPasslabel, *EnterUserlabel;
    GtkWidget *loginbackground;   
    GtkWidget *userbutton, *passbutton, *backbutton, *loginbutton;
    GtkWidget *userbuttonimage, *passbuttonimage, *backbuttonimage, *loginbuttonimage; 

    /*Initializes the window*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 800, 600); 
    gtk_container_set_border_width(GTK_CONTAINER(window), 10); 
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
    gtk_window_set_title(GTK_WINDOW(window), "Check-Mates's Chat Login"); 
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 

    /*Thisis for when you press "X" of the window, it closes it*/
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    loginlayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), loginlayout);
    gtk_widget_show(loginlayout);

    /*Creates the labels which will be put over the buttons*/ 
    backlabel = gtk_label_new("Back");
    loginlabel = gtk_label_new("Login");
    EnterUserlabel = gtk_label_new("Enter Username"); 
    EnterPasslabel = gtk_label_new("Enter Password");  

    /*This is the images for background of the layout and buttons*/
    loginbackground = gtk_image_new_from_file("./background/chatback.png");
    loginbuttonimage = gtk_image_new_from_file("./background/loginwood.png");
    userbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    passbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    backbuttonimage = gtk_image_new_from_file("./background/backlwood.png");
    
    /*This is the background*/
    gtk_layout_put(GTK_LAYOUT(loginlayout), loginbackground, 0, 0);

    /*First button which just says username*/
    userbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(loginlayout), userbutton, 300, 200);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(loginlayout), EnterUserlabel, 350 ,210);
    gtk_widget_set_size_request(userbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(userbutton), userbuttonimage);//sets the image of the wood grain inside the button

    /*Second button which just says Password*/
    passbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(loginlayout), passbutton, 300, 300);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(loginlayout), EnterPasslabel, 350, 310);
    gtk_widget_set_size_request(passbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(passbutton), passbuttonimage);//sets the image of the wood grain inside the button
 
    /*Third button which takes the user back to main*/
    backbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(StartMenu), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(loginlayout), backbutton, 0, 0);//puts the button on the layout first number is the y axis
    gtk_layout_put(GTK_LAYOUT(loginlayout), backlabel, 15, 5);//places label on top of button
    gtk_widget_set_size_request(backbutton, 70, 30);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(backbutton), backbuttonimage);//sets the image of the wood grain inside the button
    
    /*This creates a texbox for the user to enter their username*/
    usernameentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(usernameentry), 0);
    gtk_layout_put(GTK_LAYOUT(loginlayout), usernameentry, 330,235); 
   
    /*This creates a textbox for the user to enter their password*/
    passwordentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(passwordentry), 0);
    gtk_layout_put(GTK_LAYOUT(loginlayout), passwordentry, 330, 330); 
    gtk_entry_set_visibility(GTK_ENTRY(passwordentry), FALSE);

    /*Fourth button which takes user info and checks if its correct to login*/
    loginbutton = gtk_button_new_with_label(NULL);
    g_signal_connect(loginbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(loginbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(loginbutton, "clicked", G_CALLBACK(sendLoginRequest), NULL);
    gtk_layout_put(GTK_LAYOUT(loginlayout), loginbutton, 368, 400);//first number is X axis, second is Y axis
    gtk_layout_put(GTK_LAYOUT(loginlayout), loginlabel, 405, 410);
    gtk_widget_set_size_request(loginbutton, 90, 35);//first number is width, second is height
    gtk_button_set_image(GTK_BUTTON(loginbutton), loginbuttonimage);

    g_signal_connect(passwordentry, "activate", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(passwordentry, "activate",G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(passwordentry, "activate", G_CALLBACK(sendLoginRequest), NULL);


    gtk_widget_show_all(window); 
}

void StartMenu()
{
    GtkWidget *window;
    GtkWidget *loginbutton, *registerbutton;
    GtkWidget *layout;
    GtkWidget *backgroundimage, *loginbuttonimage, *registerbuttonimage;
    GtkWidget *loginlabel, *registerlabel;
 
    /*Initialize a window*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);    
    gtk_widget_set_size_request(window, 800, 600); 
    gtk_container_set_border_width(GTK_CONTAINER(window), 10); 
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
    gtk_window_set_title(GTK_WINDOW(window), "Check-Mate's Chat Program!"); 
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 

    /*This is for when you press the "X" of the window it closes it*/
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    /*The initialization of GTK Layout instead of a usual Table*/
    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show(layout);

    /*Creates the labels which will be put over the buttons*/
    loginlabel = gtk_label_new("Login");
    
    registerlabel = gtk_label_new("Register");
 
    /*This is the image background in the form of a layout instead of a table*/
    backgroundimage = gtk_image_new_from_file("./background/chatback.png");
    loginbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    registerbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    gtk_layout_put(GTK_LAYOUT(layout), backgroundimage, 0, 0); 

    /*First button which just says login*/
    loginbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(loginbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(loginbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(loginbutton, "clicked", G_CALLBACK(Login), NULL);    
    gtk_layout_put(GTK_LAYOUT(layout), loginbutton, 300, 200);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(layout), loginlabel, 390, 235);//places label on top of button
    gtk_widget_set_size_request(loginbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(loginbutton), loginbuttonimage);//sets the image of the wood grain inside the button

    /*Second button which just says register*/    
    registerbutton = gtk_button_new_with_label(NULL); //creates button
    g_signal_connect(registerbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(registerbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(registerbutton, "clicked", G_CALLBACK(Register), NULL);    
    gtk_layout_put(GTK_LAYOUT(layout), registerbutton, 300, 300);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(layout), registerlabel, 380, 335);//places label on top of button
    gtk_widget_set_size_request(registerbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(registerbutton), registerbuttonimage);//sets the image of the wood grain inside the button
 
    /*Shows everything*/
    gtk_widget_show_all(window);

    gtk_main();

}


void Register()
{
    GtkWidget *window;
    GtkWidget *registerlayout;
    GtkWidget *backlabel, *registerlabel, *EnternUserlabel, *EnternPasslabel;
    GtkWidget *registerbackground;
    GtkWidget *userbutton, *passbutton, *backbutton, *regbutton;
    GtkWidget *userbuttonimage, *passbuttonimage, *backbuttonimage, *regbuttonimage;

    /*Initializes the window*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Check-Mates's Chat Register");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /*Thisis for when you press "X" of the window, it closes it*/
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(destroy), NULL);

    /*The initialization of GTK layout instead of the usual table*/
    registerlayout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), registerlayout);
    gtk_widget_show(registerlayout);

    /*Creates the labels which will be put over the buttons*/
    backlabel = gtk_label_new("Back");
    registerlabel = gtk_label_new("Register");
    EnternUserlabel = gtk_label_new("Enter New Username");
    EnternPasslabel = gtk_label_new("Enter New Password");

    /*This is the image background in the form of a layout instead of a table*/
    registerbackground = gtk_image_new_from_file("./background/chatback.png");
    userbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    passbuttonimage = gtk_image_new_from_file("./background/lwood.png");
    backbuttonimage = gtk_image_new_from_file("./background/backlwood.png");
    regbuttonimage = gtk_image_new_from_file("./background/registerwood.png");
    gtk_layout_put(GTK_LAYOUT(registerlayout), registerbackground, 0, 0);
  
    /*First button which just will ask for a username*/
    userbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(registerlayout), userbutton, 300, 200);//puts the button on the layout 
    gtk_layout_put(GTK_LAYOUT(registerlayout), EnternUserlabel, 340 ,210);
    gtk_widget_set_size_request(userbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(userbutton), userbuttonimage);//sets the image of the wood grain inside the button

    /*Second button which just will ask for a password*/
    passbutton = gtk_button_new_with_label(NULL);//creates button
    gtk_layout_put(GTK_LAYOUT(registerlayout), passbutton, 300, 300);//puts the button on the layout
    gtk_layout_put(GTK_LAYOUT(registerlayout), EnternPasslabel, 340 ,310);
    gtk_widget_set_size_request(passbutton, 215, 90);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(passbutton), passbuttonimage);//sets the image of the wood grain inside the button

    /*Third button which takes the user back to main*/
    backbutton = gtk_button_new_with_label(NULL);//creates button
    g_signal_connect(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(backbutton, "clicked", G_CALLBACK(StartMenu), NULL);//When the back button is clicked it goes back to main
    gtk_layout_put(GTK_LAYOUT(registerlayout), backbutton, 0, 0);//puts the button on the layout first number is the y axis
    gtk_layout_put(GTK_LAYOUT(registerlayout), backlabel, 15, 5);//places label on top of button
    gtk_widget_set_size_request(backbutton, 70, 30);//this tells us how big the button will be
    gtk_button_set_image(GTK_BUTTON(backbutton), backbuttonimage);//sets the image of the wood grain inside the button

    /*This creates a texbox for the user to enter their username*/
    usernameregentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(usernameregentry), 0);
    gtk_layout_put(GTK_LAYOUT(registerlayout), usernameregentry, 330,235); 

    /*This creates a textbox for the user to enter their password*/
    passwordregentry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(passwordregentry), 0);
    gtk_layout_put(GTK_LAYOUT(registerlayout), passwordregentry, 330, 330);
    gtk_entry_set_visibility(GTK_ENTRY(passwordregentry), FALSE);
  
    /*Fourth button which takes user info and checks if its correct to login*/
    regbutton = gtk_button_new_with_label(NULL);
    g_signal_connect(regbutton, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(regbutton, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(regbutton, "clicked", G_CALLBACK(sendRegisterRequest), NULL);
    gtk_layout_put(GTK_LAYOUT(registerlayout), regbutton, 368, 400);//first number is X axis, second is Y axis
    gtk_layout_put(GTK_LAYOUT(registerlayout), registerlabel, 385, 410);
    gtk_widget_set_size_request(regbutton, 90, 35);//first number is width, second is height
    gtk_button_set_image(GTK_BUTTON(regbutton), regbuttonimage);
 
    g_signal_connect(passwordregentry, "activate", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(passwordregentry, "activate",G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(passwordregentry, "activate", G_CALLBACK(sendRegisterRequest), NULL);



    gtk_widget_show_all(window); 
}

void FatalError(const char *Program, const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!", stderr);
    exit(20);
} /* end of FatalError */


void TranslateServerResponse(char ServerResponse[256])
{
    if(0 == strcmp("OKRG", ServerResponse)){printf("\nRegister Success! \n");}
    if(0 == strcmp("INRG", ServerResponse)){printf("\nInvalid Username or Password! \n");}
    if(0 == strcmp("ALEX", ServerResponse)){printf("\nUsername Already Exists! \n");}
    if(0 == strcmp("OKLN", ServerResponse)){printf("\nLogin Success! \n");}
    if(0 == strcmp("INLN", ServerResponse)){printf("\nInvalid Username or Password! \n");}
    if(0 == strcmp("WPAS", ServerResponse)){printf("\nWrong Password! \n");}

}


int main(int argc, char*argv[]){ 

/********** Connecting to server ***********/
    program = argv[0];

    printf("%s: Starting...\n", argv[0]);
    if (argc < 3)
    {   fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
	exit(10);
    }
#ifdef DEBUG
    printf("%s: Looking up host %s on the network...\n", argv[0], argv[1]);
#endif
    Server = gethostbyname(argv[1]);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", argv[0], argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);
#ifdef DEBUG
    printf("%s: Using port %d...\n", argv[0], PortNo);
#endif
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be greater 2000\n",
		argv[0], PortNo);
        exit(10);
    }
#ifdef DEBUG
    printf("%s: Creating a socket...\n", argv[0]);
#endif
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {   FatalError(argv[0], "socket creation failed");
    }
#ifdef DEBUG
    printf("%s: Preparing the server address...\n", argv[0]);
#endif
    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    memcpy(&ServerAddress.sin_addr.s_addr,
		Server->h_addr_list[0], Server->h_length);
    printf("%s: Connecting to the server...\n", argv[0]);
    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) < 0)
    {   FatalError(argv[0], "connecting to server failed");
    }

   

/********************************************/   
    gtk_init(&argc, &argv);    
  
    int t1 ,t2;  
    pthread_t receiving_thread, sending_thread;

    t1=pthread_create( &receiving_thread, NULL, 
        (void*)recvServer, (void*) &SocketFD);
    t2=pthread_create( &sending_thread, NULL,
        (void*)StartMenu, (void*) &SocketFD);

    pthread_join( receiving_thread, NULL);



    return 0;
}
