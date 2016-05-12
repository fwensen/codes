#include<stdio.h>
#include<gtk/gtk.h>

typedef struct {

    GtkWidget *window;

} window_data;

void close_app_exit(GtkWidget *window, gpointer data) {

    gtk_main_quit();

}

void click_button(GtkWidget *button, gpointer data) {

    window_data *dd = (window_data *)data;
    gtk_window_resize(GTK_WINDOW(dd->window), 1, 1);
}


int main(int argc, char *argv[]) {


    GtkWidget *window;
    GtkWidget *l_ip, *l_port, *l_groupname;
    GtkWidget *e_ip, *e_port, *e_groupname;
    GtkWidget *login_button;
    GtkWidget *hbox1, *hbox2, *hbox3;
    GtkWidget *hbox4, hbox5;
    GtkWidget *vbox;
    //GtkWidget 



    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Login!");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);

    g_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(close_app_exit), NULL);

    l_ip = gtk_label_new("Server IP: ");
    l_port = gtk_label_new("Server Port: ");
    l_groupname = gtk_label_new("Group name: ");

    e_ip = gtk_entry_new();
    e_port = gtk_entry_new();
    e_groupname = gtk_entry_new();

    login_button = gtk_button_new_with_label("Login");

    hbox1 = gtk_hbox_new(TRUE, 0);
    hbox2 = gtk_hbox_new(TRUE, 0);
    hbox3 = gtk_hbox_new(TRUE, 0);
    hbox4 = gtk_hbox_new(TRUE, 0);
    hbox5 = gtk_hbox_new(TRUE, 0);

    vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(hbox1), l_ip, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), e_ip, TRUE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(hbox2), l_port, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2), e_port, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox3), l_groupname, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox3), e_groupname, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox4), hbox1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox4), hbox2, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox5), hbox3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox5), login_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox5, FALSE, FALSE, 0);

    window_data data;
    data.window = window;
    g_signal_connect(GTK_OBJECT(login_button), "clicked", GTK_SIGNAL_FUNC(click_button), &data);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
