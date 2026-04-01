#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Структуры

typedef enum {
    MSG_TEXT,
    MSG_IMAGE,
    MSG_FILE,
    MSG_LOCATION,
    MSG_AUDIO
} MessageType;

typedef struct {
    char content[500];
    int is_edited;
} TextMessage;

typedef struct {
    char filename[100];
    int width;
    int height;
    size_t size_bytes;
} ImageMessage;

typedef struct {
    char filename[100];
    char mime_type[50];
    size_t size_bytes;
} FileMessage;

typedef struct {
    double latitude;
    double longitude;
    char address[200];
} LocationMessage;

typedef struct {
    char filename[100];
    int duration_seconds;
    size_t size_bytes;
} AudioMessage;

typedef struct {
    unsigned int id;
    unsigned int sender_id;
    unsigned int chat_id;
    time_t timestamp;
    MessageType type;
    union {
        TextMessage text;
        ImageMessage image;
        FileMessage file;
        LocationMessage location;
        AudioMessage audio;
    } content;
} Message;

// Создание сообщений

Message create_text_message(unsigned int sender, unsigned int chat, const char *text) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_TEXT;
    strncpy(msg.content.text.content, text, 499);
    msg.content.text.content[499] = '\0';
    msg.content.text.is_edited = 0;
    return msg;
}

Message create_image_message(unsigned int sender, unsigned int chat, 
                              const char *filename, int w, int h, size_t size) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_IMAGE;
    strncpy(msg.content.image.filename, filename, 99);
    msg.content.image.filename[99] = '\0';
    msg.content.image.width = w;
    msg.content.image.height = h;
    msg.content.image.size_bytes = size;
    return msg;
}

Message create_location_message(unsigned int sender, unsigned int chat,
                                 double lat, double lon, const char *address) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_LOCATION;
    msg.content.location.latitude = lat;
    msg.content.location.longitude = lon;
    strncpy(msg.content.location.address, address, 199);
    msg.content.location.address[199] = '\0';
    return msg;
}

// Вывод

const char* message_type_to_string(MessageType type) {
    switch (type) {
        case MSG_TEXT:     return "Текст";
        case MSG_IMAGE:    return "Изображение";
        case MSG_FILE:     return "Файл";
        case MSG_LOCATION: return "Локация";
        case MSG_AUDIO:    return "Аудио";
        default:           return "Неизвестно";
    }
}

void message_print(const Message *msg) {
    if (msg == NULL) return;
    
    struct tm *tm_info = localtime(&msg->timestamp);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M", tm_info);
    
    printf("[%s] User#%u: ", time_str, msg->sender_id);
    
    switch (msg->type) {
        case MSG_TEXT:
            printf("%s\n", msg->content.text.content);
            break;
            
        case MSG_IMAGE:
            printf("[Изображение: %s (%dx%d, %.1f KB)]\n", 
                   msg->content.image.filename,
                   msg->content.image.width, msg->content.image.height,
                   msg->content.image.size_bytes / 1024.0);
            break;
            
        case MSG_LOCATION:
            printf("[Локация: %.4f, %.4f - %s]\n",
                   msg->content.location.latitude,
                   msg->content.location.longitude,
                   msg->content.location.address);
            break;
            
        default:
            printf("[Сообщение типа %s]\n", message_type_to_string(msg->type));
            break;
    }
}

// Фильтрация массива сообщений

size_t filter_messages_by_type(const Message *messages, size_t count, 
                                MessageType type, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].type == type) {
            result[found++] = messages[i];
        }
    }
    return found;
}

size_t filter_messages_by_sender(const Message *messages, size_t count,
                                  unsigned int sender_id, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].sender_id == sender_id) {
            result[found++] = messages[i];
        }
    }
    return found;
}

// Функция MAIN

int main(void) {
    srand(time(NULL));
    
    Message chat[100];
    int msg_count = 0;
    
    chat[msg_count++] = create_text_message(1, 42, "Привет! Как дела?");
    chat[msg_count++] = create_text_message(2, 42, "Отлично! Смотри что нашёл:");
    chat[msg_count++] = create_image_message(2, 42, "photo.jpg", 1920, 1080, 2500000);
    chat[msg_count++] = create_text_message(1, 42, "Круто! Где это?");
    chat[msg_count++] = create_location_message(2, 42, 55.7558, 37.6173, "Москва, Красная площадь");
    
    printf("=== Чат #42 ===\n");
    for (int i = 0; i < msg_count; i++) {
        message_print(&chat[i]);
    }
    
    // Фильтрация текстовых сообщений
    Message text_msgs[100];
    size_t text_count = filter_messages_by_type(chat, msg_count, MSG_TEXT, text_msgs);
    
    printf("\n=== Текстовые сообщения ===\n");
    for (size_t i = 0; i < text_count; i++) {
        message_print(&text_msgs[i]);
    }
    
    return 0;
}
