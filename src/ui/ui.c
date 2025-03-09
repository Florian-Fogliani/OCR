#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <stdio.h>

struct Controls
{
    GtkFileChooserButton *select_file_Button;

    GtkButton *confirm_image_button;
    GtkFileChooserButton *select_another_file_button;

    GtkButton *filter_button;
    GtkSpinButton *rotation_button;
    GtkButton *automatically_rotation_button;
    GtkButton *next_stape_p3_button;

    GtkButton *cut_button;
    GtkButton *next_stape_p4_button;

    GtkButton *solve_button;
};
typedef struct Controls Controls;

struct Page
{
    GtkViewport *container;
    GtkImage *image;
};
typedef struct Page Page;

struct Pages
{
    Page *page1;
    Page *page2;
    Page *page3;
    Page *page4;
    Page *page5;
};
typedef struct Pages Pages;

struct Images
{
    char *path_original;
    char *path_filtered;
    char *path_rotated;
    char *path_cutted;
    char *path_solved;
};
typedef struct Images Images;
struct MainWindow
{
    Images *images;
    GtkWidget *window;
    GtkStack *stack;
    Controls *controls;
    Pages *pages;
    int actu_page;
};
typedef struct MainWindow MainWindow;

MainWindow main_window;

void display(char *path)
{
    GtkImage *i;
    switch (main_window.actu_page)
    {
    case 2:
        i = main_window.pages->page2->image;
        break;
    case 3:
        i = main_window.pages->page3->image;
        break;
    case 4:
        i = main_window.pages->page4->image;
        break;
    case 5:
        i = main_window.pages->page5->image;
        break;
    default:
        return;
    }
    GError *e = NULL;
    GdkPixbuf *buf = gdk_pixbuf_new_from_file_at_size(path, 500, 500, &e);
    if (buf == NULL)
    {
        g_printerr("Error loading file:%s\n", e->message);
        g_error_free(e);
        exit(1);
    }
    gtk_image_set_from_pixbuf(i, buf);
}

void set_page(gchar *page)
{
    gtk_stack_set_visible_child_name(main_window.stack, page);
}

void file_selected(GtkFileChooserButton *file_chooser)
{
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser));
    char *path = g_file_get_path(file);
    // g_print("%s",path);
    main_window.images->path_original = path;
    set_page("page1");
    main_window.actu_page = 2;
    display(path);
}

void next_page()
{
    main_window.actu_page += 1;
    switch (main_window.actu_page)
    {
    case 3:
        set_page("page2");
        // g_print("%s",main_window.images->path_original);
        display(main_window.images->path_original);
        break;
    case 4:
        set_page("page3");
        // display(main_window.images->path_rotated);
        break;
    case 5:
        set_page("page4");
        // display(main_window.images->path_cutted);
        break;
    default:
        break;
    }
}

void window_destroy(GtkWidget *_)
{
    //	free(main_window);

    gtk_main_quit();
}

int main()
{
    gtk_init(NULL, NULL);

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "windows"));
    GtkFileChooserButton *select_file_Button = GTK_FILE_CHOOSER_BUTTON(
        gtk_builder_get_object(builder, "SelectFileButton"));
    GtkButton *confirm_image_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "AgreeBoutton"));
    GtkFileChooserButton *select_another_file_Button = GTK_FILE_CHOOSER_BUTTON(
        gtk_builder_get_object(builder, "SelectAnotherFileButton"));
    GtkButton *filter_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "FilterButton"));
    GtkSpinButton *rotation_button =
        GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "AngleButton"));
    GtkButton *automatically_rotation_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "RotationButton"));
    GtkButton *next_stape_p3_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "NextStapeP3Button"));
    GtkButton *cut_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "CutButton"));
    GtkButton *next_stape_p4_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "NextStapeP4Button"));
    GtkButton *solve_button =
        GTK_BUTTON(gtk_builder_get_object(builder, "SolveButton"));

    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    GtkViewport *page1_container =
        GTK_VIEWPORT(gtk_builder_get_object(builder, "page1"));
    GtkViewport *page2_container =
        GTK_VIEWPORT(gtk_builder_get_object(builder, "page2"));
    GtkViewport *page3_container =
        GTK_VIEWPORT(gtk_builder_get_object(builder, "page3"));
    GtkViewport *page4_container =
        GTK_VIEWPORT(gtk_builder_get_object(builder, "page4"));
    GtkViewport *page5_container =
        GTK_VIEWPORT(gtk_builder_get_object(builder, "page5"));

    GtkImage *page2_image =
        GTK_IMAGE(gtk_builder_get_object(builder, "ImagePage2"));
    GtkImage *page3_image =
        GTK_IMAGE(gtk_builder_get_object(builder, "ImagePage3"));
    GtkImage *page4_image =
        GTK_IMAGE(gtk_builder_get_object(builder, "ImagePage4"));
    GtkImage *page5_image =
        GTK_IMAGE(gtk_builder_get_object(builder, "ImagePage5"));

    Controls controls = {
        .select_file_Button = select_file_Button,

        .confirm_image_button = confirm_image_button,
        .select_another_file_button = select_another_file_Button,

        .filter_button = filter_button,
        .rotation_button = rotation_button,
        .automatically_rotation_button = automatically_rotation_button,
        .next_stape_p3_button = next_stape_p3_button,

        .cut_button = cut_button,
        .next_stape_p4_button = next_stape_p4_button,

        .solve_button = solve_button,
    };
    Page page1 = {
        .container = page1_container,
        .image = NULL,
    };
    Page page2 = {
        .container = page2_container,
        .image = page2_image,
    };
    Page page3 = {
        .container = page3_container,
        .image = page3_image,
    };
    Page page4 = {
        .container = page4_container,
        .image = page4_image,
    };
    Page page5 = {
        .container = page5_container,
        .image = page5_image,
    };
    Pages pages = {
        .page1 = &page1,
        .page2 = &page2,
        .page3 = &page3,
        .page4 = &page4,
        .page5 = &page5,
    };
    Images images = {
        .path_original = NULL,
        .path_filtered = NULL,
        .path_rotated = NULL,
        .path_cutted = NULL,
        .path_solved = NULL,
    };
    main_window.images = &images, main_window.window = GTK_WIDGET(window);
    main_window.stack = stack;
    main_window.controls = &controls;
    main_window.pages = &pages;
    main_window.actu_page = 1;

    gtk_widget_show_all(GTK_WIDGET(window));
    // g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(window_destroy),
                     &main_window);

    g_signal_connect(main_window.controls->select_file_Button, "file-set",
                     G_CALLBACK(file_selected), &main_window);

    g_signal_connect(main_window.controls->select_another_file_button,
                     "file-set", G_CALLBACK(file_selected), &main_window);
    g_signal_connect(main_window.controls->confirm_image_button, "clicked",
                     G_CALLBACK(next_page), &main_window);

    g_signal_connect(main_window.controls->next_stape_p3_button, "clicked",
                     G_CALLBACK(next_page), &main_window);

    g_signal_connect(main_window.controls->next_stape_p4_button, "clicked",
                     G_CALLBACK(next_page), &main_window);

    gtk_main();
}
