#pragma once

void api_messaging_create();
void api_messaging_destroy();

void send_api_message_with_callback(char *command, char *data, void *callback);
void send_api_message(char *command, char *data);
