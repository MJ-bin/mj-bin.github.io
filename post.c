#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_DATE_BUF_SIZE 16
#define POST_DATE_BUF_SIZE 32
#define SLUG_BUF_SIZE 64
#define PATH_BUF_SIZE 96
#define LINE_BUF_SIZE 256

void get_file_date_str(char* buf, size_t buf_size);
void get_post_date_str(char* buf, size_t buf_size);
void create_image_dirs(char* post_path);
void copy_template(char* file_date, char* post_date);
void delete_post(char* file_date, int index);


int main(int argc, char *argv[]) {
    char file_date[FILE_DATE_BUF_SIZE];
    char post_date[POST_DATE_BUF_SIZE];

    if (argc < 2) {
        printf("usage: ./post <new> | ./post delete <index>\n");
        return 0;
    }

    get_file_date_str(file_date, sizeof(file_date));

    if (strcmp(argv[1], "new") == 0) {
        get_post_date_str(post_date, sizeof(post_date));
        copy_template(file_date, post_date);
        return 0;
    }

    if (strcmp(argv[1], "delete") == 0 && argc >= 3) {
        delete_post(file_date, atoi(argv[2]));
        return 0;
    }

    printf("usage: ./post <new> | ./post delete <index>\n");

    return 0;
}


void get_file_date_str(char* buf, size_t buf_size) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    strftime(buf, buf_size, "%Y-%m-%d", tm_now);
}


void get_post_date_str(char* buf, size_t buf_size) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S %z", tm_now);
}


void create_image_dirs(char* post_path) {
    char slug[SLUG_BUF_SIZE];
    char covers_dir[PATH_BUF_SIZE];
    char posts_dir[PATH_BUF_SIZE];
    char *filename = strrchr(post_path, '/');
    char *ext = NULL;

    if (filename == NULL) {
        filename = post_path;
    } else {
        filename++;
    }

    snprintf(slug, sizeof(slug), "%s", filename);
    ext = strrchr(slug, '.');
    if (ext != NULL) {
        *ext = '\0';
    }

    snprintf(covers_dir, sizeof(covers_dir), "./assets/img/covers/%s", slug);
    snprintf(posts_dir, sizeof(posts_dir), "./assets/img/posts/%s", slug);

    if (mkdir(covers_dir, 0755) != 0) {
        perror("covers dir create failed");
    }

    if (mkdir(posts_dir, 0755) != 0) {
        perror("posts dir create failed");
    }
}


void delete_post(char* file_date, int index) {
    char slug[SLUG_BUF_SIZE];
    char post_path[PATH_BUF_SIZE];
    char covers_dir[PATH_BUF_SIZE];
    char posts_dir[PATH_BUF_SIZE];

    if (index < 0) {
        printf("index must be 0 or greater\n");
        return;
    }

    if (index == 0) {
        snprintf(slug, sizeof(slug), "%s-new-post", file_date);
    } else {
        snprintf(slug, sizeof(slug), "%s-new-post-%d", file_date, index);
    }

    snprintf(post_path, sizeof(post_path), "./_posts/%s.md", slug);
    snprintf(covers_dir, sizeof(covers_dir), "./assets/img/covers/%s", slug);
    snprintf(posts_dir, sizeof(posts_dir), "./assets/img/posts/%s", slug);

    if (remove(post_path) != 0) {
        perror("post delete failed");
    }

    if (rmdir(covers_dir) != 0) {
        perror("covers dir delete failed");
    }

    if (rmdir(posts_dir) != 0) {
        perror("posts dir delete failed");
    }
}


void copy_template(char* file_date, char* post_date) {
    FILE *src = fopen("./templates/post-skeleton.md", "r"); 
    char path_buf[PATH_BUF_SIZE];
    char line[LINE_BUF_SIZE];
    int suffix = 0;

    if (src == NULL) {
        perror("Failed to open template file");
        return;
    }

    snprintf(path_buf, sizeof(path_buf), "./_posts/%s-new-post.md", file_date);

    while (access(path_buf, F_OK) == 0) {
        suffix++;
        snprintf(path_buf, sizeof(path_buf), "./_posts/%s-new-post-%d.md", file_date, suffix);
    }

    FILE *dst = fopen(path_buf, "w");
    if (dst == NULL) {
        perror("new post open failed");
        fclose(src);
        return;
    }

    while (fgets(line, sizeof(line), src) != NULL) {
        if (strncmp(line, "date:", 5) == 0) {
            fprintf(dst, "date: %s\n", post_date);
        } else {
            fputs(line, dst);
        }
    }

    fclose(src);
    fclose(dst);
    create_image_dirs(path_buf);

    printf("created: %s\n", path_buf);
}
