#include <gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 300

#define WIDGET_SPACING 10
#define WIDGET_MARGIN 10

#define INFO_PATTERN                                                \
  "\nCapital: $%.2f\nPosition: %.2f%%\nPrice: $%.2f\nShare: %d\n"   \
  "\nStop:\n3%%: $%.2f\n5%%: "                                      \
  "$%.2f\n7%%: "                                                    \
  "$%.2f\n\nProfit:\n10%%: $%.2f\n15%%: $%.2f\n20%%: $%.2f\n25%%: " \
  "$%.2f\n30%%: "                                                   \
  "$%.2f\n\n%s\n"

#define INFO_QUOTE "Succeed!  No Matter What!  Whatever It Takes!"

#define INFO_ERROR "I should study harder to solve to question!"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static gboolean strIsDigit(const gchar* str) {
  guint length = strlen(str);

  for (unsigned int i = 0; i < length; i++) {
    if (!isdigit(str[i])) {
      if (str[i] != '.') {
        return FALSE;
      }
    }
  }

  return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UserData {
  GtkWidget* mainWindow;

  GtkWidget* entryCapital;
  GtkWidget* entryPrice;
  GtkWidget* entryPosition;

  GtkWidget* labelInfo;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void buttonClicked(GtkButton* button, void* data) {
  struct UserData* userData = (struct UserData*)data;

  gchar* info;

  const gchar* charCapital =
      gtk_entry_get_text(GTK_ENTRY(userData->entryCapital));
  const gchar* charPrice = gtk_entry_get_text(GTK_ENTRY(userData->entryPrice));
  const gchar* charPosition =
      gtk_entry_get_text(GTK_ENTRY(userData->entryPosition));

  if (strlen(charCapital) == 0 || strlen(charPrice) == 0 ||
      strlen(charPosition) == 0) {
    goto error;
  }

  if (!strIsDigit(charCapital) || !strIsDigit(charPrice) ||
      !strIsDigit(charPosition)) {
    goto error;
  }

  float capital = atof(charCapital);
  float price = atof(charPrice);
  float position = atof(charPosition);

  float amount = capital * (position / 100.0);
  int share = amount / price;

  float three = price * (1.0 - 0.03);
  float five = price * (1.0 - 0.05);
  float seven = price * (1.0 - 0.07);

  float ten = price * (1.0 + 0.1);
  float fifteen = price * (1.0 + 0.15);
  float twenty = price * (1.0 + 0.2);
  float twentyfive = price * (1.0 + 0.25);
  float thirty = price * (1.0 + 0.3);

  info = g_strdup_printf(INFO_PATTERN, capital, position, price, share, three,
                         five, seven, ten, fifteen, twenty, twentyfive, thirty,
                         INFO_QUOTE);

  gtk_label_set_text(GTK_LABEL(userData->labelInfo), info);

  goto cleanup;

error:

  info = g_strdup_printf(INFO_PATTERN, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, INFO_ERROR);

  gtk_label_set_text(GTK_LABEL(userData->labelInfo), info);

  goto cleanup;

cleanup:
  g_free(info);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void activate() {
  GtkWidget* mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(mainWindow), "Stock Calculator");
  gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);

  gtk_widget_set_size_request(mainWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

  gtk_window_set_gravity(GTK_WINDOW(mainWindow), GDK_GRAVITY_SOUTH_WEST);
  gtk_window_move(GTK_WINDOW(mainWindow), 0, 1080);

  gtk_window_set_keep_above(GTK_WINDOW(mainWindow), TRUE);

  GtkWidget* layoutBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, WIDGET_SPACING);

  gtk_widget_set_margin_bottom(layoutBox, WIDGET_MARGIN);
  gtk_widget_set_margin_top(layoutBox, WIDGET_MARGIN);
  gtk_widget_set_margin_start(layoutBox, WIDGET_MARGIN);
  gtk_widget_set_margin_end(layoutBox, WIDGET_MARGIN);

  GtkWidget* labelCapital = gtk_label_new("Capital($):");
  gtk_widget_set_halign(labelCapital, GTK_ALIGN_START);

  GtkWidget* labelPrice = gtk_label_new("Price($):");
  gtk_widget_set_halign(labelPrice, GTK_ALIGN_START);

  GtkWidget* labelPosition = gtk_label_new("Position(%):");
  gtk_widget_set_halign(labelPosition, GTK_ALIGN_START);

  GtkWidget* entryCapital = gtk_entry_new();
  GtkWidget* entryPrice = gtk_entry_new();
  GtkWidget* entryPosition = gtk_entry_new();

  GtkWidget* buttonCalculate = gtk_button_new_with_label("Calculate");

  gchar* info = g_strdup_printf(INFO_PATTERN, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0,
                                0.0, 0.0, 0.0, 0.0, 0.0, INFO_QUOTE);
  GtkWidget* labelInfo = gtk_label_new(info);
  gtk_widget_set_halign(labelInfo, GTK_ALIGN_START);

  g_free(info);

  gtk_container_add(GTK_CONTAINER(layoutBox), labelCapital);
  gtk_container_add(GTK_CONTAINER(layoutBox), entryCapital);

  gtk_container_add(GTK_CONTAINER(layoutBox), labelPosition);
  gtk_container_add(GTK_CONTAINER(layoutBox), entryPosition);

  gtk_container_add(GTK_CONTAINER(layoutBox), labelPrice);
  gtk_container_add(GTK_CONTAINER(layoutBox), entryPrice);

  gtk_container_add(GTK_CONTAINER(layoutBox), buttonCalculate);

  gtk_container_add(GTK_CONTAINER(layoutBox), labelInfo);

  gtk_container_add(GTK_CONTAINER(mainWindow), layoutBox);

  struct UserData* data = malloc(sizeof(struct UserData));

  data->mainWindow = mainWindow;
  data->entryCapital = entryCapital;
  data->entryPrice = entryPrice;
  data->entryPosition = entryPosition;
  data->labelInfo = labelInfo;

  g_signal_connect(buttonCalculate, "clicked", G_CALLBACK(buttonClicked), data);

  g_signal_connect(GTK_WIDGET(mainWindow), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);

  gtk_widget_show_all(GTK_WIDGET(mainWindow));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  gtk_init(&argc, &argv);

  activate();

  gtk_main();

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
