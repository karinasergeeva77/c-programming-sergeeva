#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define IMG_MAGIC 0x494D4731  // "IMG1"

// Структуры

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint8_t bits_per_pixel;  // 8 (grayscale), 24 (RGB), 32 (RGBA)
    uint8_t compression;      // 0 = none
    uint16_t reserved;
} ImageHeader;

typedef struct {
    uint8_t r, g, b, a;
} Pixel;

typedef struct {
    ImageHeader header;
    Pixel *pixels;
} Image;

// Создание

Image* image_create(int width, int height, int bits_per_pixel) {
    if (width <= 0 || height <= 0) return NULL;
    
    Image *img = malloc(sizeof(Image));
    if (img == NULL) return NULL;
    
    img->header.magic = IMG_MAGIC;
    img->header.width = width;
    img->header.height = height;
    img->header.bits_per_pixel = bits_per_pixel;
    img->header.compression = 0;
    img->header.reserved = 0;
    
    int pixel_count = width * height;
    img->pixels = malloc(pixel_count * sizeof(Pixel));
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }
    
    // Инициализация чёрным цветом
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = 0;
        img->pixels[i].g = 0;
        img->pixels[i].b = 0;
        img->pixels[i].a = 255;
    }
    
    return img;
}

// Загрузка

Image* image_load(const char *filename) {
    if (filename == NULL) return NULL;
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }
    
    Image *img = malloc(sizeof(Image));
    if (img == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Чтение заголовка
    fread(&img->header, sizeof(ImageHeader), 1, file);
    
    if (img->header.magic != IMG_MAGIC) {
        fprintf(stderr, "Некорректный формат изображения\n");
        fclose(file);
        free(img);
        return NULL;
    }
    
    int pixel_count = img->header.width * img->header.height;
    img->pixels = malloc(pixel_count * sizeof(Pixel));
    if (img->pixels == NULL) {
        fclose(file);
        free(img);
        return NULL;
    }
    
    // Чтение пикселей
    fread(img->pixels, sizeof(Pixel), pixel_count, file);
    
    fclose(file);
    printf("Загружено изображение %dx%d\n", img->header.width, img->header.height);
    return img;
}

// Сохранение

int image_save(const Image *img, const char *filename) {
    if (img == NULL || filename == NULL) return -1;
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Ошибка создания файла");
        return -1;
    }
    
    // Запись заголовка
    fwrite(&img->header, sizeof(ImageHeader), 1, file);
    
    // Запись пикселей
    int pixel_count = img->header.width * img->header.height;
    fwrite(img->pixels, sizeof(Pixel), pixel_count, file);
    
    fclose(file);
    printf("Сохранено изображение в %s\n", filename);
    return 0;
}

void image_free(Image *img) {
    if (img == NULL) return;
    
    if (img->pixels != NULL) {
        free(img->pixels);
    }
    free(img);
}

// Доступ к пикселям

Pixel image_get_pixel(const Image *img, int x, int y) {
    Pixel p = {0, 0, 0, 255};
    if (img == NULL) return p;
    if (x < 0 || x >= (int)img->header.width) return p;
    if (y < 0 || y >= (int)img->header.height) return p;
    
    int index = y * img->header.width + x;
    return img->pixels[index];
}

void image_set_pixel(Image *img, int x, int y, Pixel p) {
    if (img == NULL) return;
    if (x < 0 || x >= (int)img->header.width) return;
    if (y < 0 || y >= (int)img->header.height) return;
    
    int index = y * img->header.width + x;
    img->pixels[index] = p;
}

// Преобразования

void image_invert(Image *img) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = 255 - img->pixels[i].r;
        img->pixels[i].g = 255 - img->pixels[i].g;
        img->pixels[i].b = 255 - img->pixels[i].b;
    }
}

void image_grayscale(Image *img) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        uint8_t gray = (img->pixels[i].r + img->pixels[i].g + img->pixels[i].b) / 3;
        img->pixels[i].r = gray;
        img->pixels[i].g = gray;
        img->pixels[i].b = gray;
    }
}

void image_brightness(Image *img, int delta) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        int r = img->pixels[i].r + delta;
        int g = img->pixels[i].g + delta;
        int b = img->pixels[i].b + delta;
        
        img->pixels[i].r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        img->pixels[i].g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        img->pixels[i].b = (b < 0) ? 0 : (b > 255) ? 255 : b;
    }
}

void image_contrast(Image *img, float factor) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        float r = (img->pixels[i].r - 128) * factor + 128;
        float g = (img->pixels[i].g - 128) * factor + 128;
        float b = (img->pixels[i].b - 128) * factor + 128;
        
        img->pixels[i].r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        img->pixels[i].g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        img->pixels[i].b = (b < 0) ? 0 : (b > 255) ? 255 : b;
    }
}

// Геометрия

Image* image_crop(const Image *img, int x, int y, int w, int h) {
    if (img == NULL) return NULL;
    
    Image *cropped = image_create(w, h, img->header.bits_per_pixel);
    if (cropped == NULL) return NULL;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel p = image_get_pixel(img, x + j, y + i);
            image_set_pixel(cropped, j, i, p);
        }
    }
    
    return cropped;
}

Image* image_rotate_90(const Image *img) {
    if (img == NULL) return NULL;
    
    Image *rotated = image_create(img->header.height, img->header.width, img->header.bits_per_pixel);
    if (rotated == NULL) return NULL;
    
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(rotated, img->header.height - 1 - i, j, p);
        }
    }
    
    return rotated;
}

void image_flip_horizontal(Image *img) {
    if (img == NULL) return;
    
    int w = img->header.width;
    int h = img->header.height;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w / 2; j++) {
            Pixel p1 = image_get_pixel(img, j, i);
            Pixel p2 = image_get_pixel(img, w - 1 - j, i);
            image_set_pixel(img, j, i, p2);
            image_set_pixel(img, w - 1 - j, i, p1);
        }
    }
}

void image_flip_vertical(Image *img) {
    if (img == NULL) return;
    
    int w = img->header.width;
    int h = img->header.height;
    
    for (int i = 0; i < h / 2; i++) {
        for (int j = 0; j < w; j++) {
            Pixel p1 = image_get_pixel(img, j, i);
            Pixel p2 = image_get_pixel(img, j, h - 1 - i);
            image_set_pixel(img, j, i, p2);
            image_set_pixel(img, j, h - 1 - i, p1);
        }
    }
}

// Рисование

void image_draw_line(Image *img, int x1, int y1, int x2, int y2, Pixel color) {
    if (img == NULL) return;
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        image_set_pixel(img, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void image_draw_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    if (img == NULL) return;
    
    image_draw_line(img, x, y, x + w, y, color);
    image_draw_line(img, x + w, y, x + w, y + h, color);
    image_draw_line(img, x + w, y + h, x, y + h, color);
    image_draw_line(img, x, y + h, x, y, color);
}

void image_fill_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    if (img == NULL) return;
    
    for (int i = y; i < y + h && i < (int)img->header.height; i++) {
        for (int j = x; j < x + w && j < (int)img->header.width; j++) {
            image_set_pixel(img, j, i, color);
        }
    }
}

// Вывод изображения

void image_print_ascii(const Image *img) {
    if (img == NULL) return;
    
    printf("Изображение %dx%d:\n", img->header.width, img->header.height);
    printf("+");
    for (int i = 0; i < (int)img->header.width; i++) printf("-");
    printf("+\n");
    
    for (int i = 0; i < (int)img->header.height; i++) {
        printf("|");
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            // Преобразуем цвет в символ
            char c = ' ';
            if (p.r + p.g + p.b > 600) c = '#';
            else if (p.r + p.g + p.b > 400) c = '*';
            else if (p.r + p.g + p.b > 200) c = '+';
            else if (p.r + p.g + p.b > 50) c = '.';
            else c = ' ';
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("+");
    for (int i = 0; i < (int)img->header.width; i++) printf("-");
    printf("+\n");
}

int main(void) {
    // Создаём папку для изображений
    system("mkdir -p images");
    
    // Создаём изображение 100x100
    printf("=== Создание изображения ===\n");
    Image *img = image_create(100, 100, 24);
    if (img == NULL) {
        printf("Ошибка создания изображения\n");
        return 1;
    }
    
    // Рисуем градиент
    printf("Рисование градиента...\n");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Pixel p = {i * 255 / 100, j * 255 / 100, (i + j) * 255 / 200, 255};
            image_set_pixel(img, j, i, p);
        }
    }
    
    // Сохраняем оригинал
    image_save(img, "images/original.img");
    
    // Инвертирование
    printf("\n=== Инвертирование ===\n");
    Image *inverted = image_create(img->header.width, img->header.height, 24);
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(inverted, j, i, p);
        }
    }
    image_invert(inverted);
    image_save(inverted, "images/inverted.img");
    image_free(inverted);
    
    // Оттенки серого
    printf("\n=== Оттенки серого ===\n");
    Image *gray = image_create(img->header.width, img->header.height, 24);
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(gray, j, i, p);
        }
    }
    image_grayscale(gray);
    image_save(gray, "images/grayscale.img");
    image_free(gray);
    
    // Поворот на 90 градусов
    printf("\n=== Поворот на 90° ===\n");
    Image *rotated = image_rotate_90(img);
    image_save(rotated, "images/rotated.img");
    image_free(rotated);
    
    // Обрезка
    printf("\n=== Обрезка ===\n");
    Image *cropped = image_crop(img, 25, 25, 50, 50);
    image_save(cropped, "images/cropped.img");
    image_free(cropped);
    
    // Создание простого рисунка
    printf("\n=== Создание рисунка ===\n");
    Image *drawing = image_create(80, 40, 24);
    Pixel white = {255, 255, 255, 255};
    Pixel red = {255, 0, 0, 255};
    Pixel blue = {0, 0, 255, 255};
    Pixel green = {0, 255, 0, 255};
    
    image_fill_rect(drawing, 0, 0, 80, 40, white);
    image_draw_rect(drawing, 10, 5, 60, 30, red);
    image_draw_line(drawing, 10, 20, 70, 20, blue);
    image_draw_line(drawing, 40, 5, 40, 35, green);
    
    image_save(drawing, "images/drawing.img");
    image_print_ascii(drawing);
    image_free(drawing);
    
    // Загрузка и проверка
    printf("\n=== Проверка загрузки ===\n");
    Image *loaded = image_load("images/original.img");
    if (loaded) {
        printf("Изображение успешно загружено!\n");
        image_free(loaded);
    }
    
    image_free(img);
    
    printf("\nВсе изображения сохранены в папке images/\n");
    return 0;
}
