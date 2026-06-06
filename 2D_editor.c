#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 22
#define COLS 65

typedef enum { LINE = 1, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

typedef struct { int x1, y1, x2, y2; } LineData;
typedef struct { int x1, y1, x2, y2; } RectData;
typedef struct { int xc, yc, r; } CircleData;
typedef struct { int x1, y1, x2, y2, x3, y3; } TriData;

typedef struct ShapeNode {
    int id;
    ShapeType type;
    union {
        LineData line;
        RectData rect;
        CircleData circle;
        TriData tri;
    } data;
    struct ShapeNode* next;
} ShapeNode;

/* Function Declarations */
void reset_canvas(char canvas[ROWS][COLS]);
void display_canvas(char canvas[ROWS][COLS]);
void plot(char canvas[ROWS][COLS], int x, int y);
void draw_line(char canvas[ROWS][COLS], int x0, int y0, int x1, int y1);
void draw_rectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void draw_circle(char canvas[ROWS][COLS], int xc, int yc, int r);
void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3);
void render_all_shapes(char canvas[ROWS][COLS], ShapeNode* head);
void add_shape(ShapeNode** head, ShapeNode* new_node);
int delete_shape(ShapeNode** head, int id);
ShapeNode* find_shape(ShapeNode* head, int id);
void list_shapes(ShapeNode* head);
void free_all_shapes(ShapeNode** head);
void save_drawing(ShapeNode* head, const char* filename);
void load_drawing(ShapeNode** head, int* next_id, const char* filename);

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    char canvas[ROWS][COLS];
    ShapeNode* head = NULL;
    int next_id = 1, choice;

    reset_canvas(canvas);

    while (1) {
        render_all_shapes(canvas, head);
        printf("\n");
        display_canvas(canvas);
        list_shapes(head);

        printf("\nMENU: 1.Add 2.Modify 3.Delete 4.Save 5.Load 6.Clear 7.Exit\nSelect choice (1-7): ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 7) {
            free_all_shapes(&head);
            printf("Exiting dynamic canvas program.\n");
            break;
        }

        switch (choice) {
            case 1: { /* Add Shape */
                int type;
                printf("Shape Type (1.Line, 2.Rect, 3.Circle, 4.Tri): ");
                if (scanf("%d", &type) != 1 || type < 1 || type > 4) {
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                ShapeNode* node = (ShapeNode*)malloc(sizeof(ShapeNode));
                if (!node) { printf("[Error] Memory allocation failed!\n"); break; }
                node->id = next_id++;
                node->type = (ShapeType)type;
                node->next = NULL;

                if (type == LINE) {
                    printf("Enter coords (X1 Y1 X2 Y2): ");
                    scanf("%d %d %d %d", &node->data.line.x1, &node->data.line.y1, &node->data.line.x2, &node->data.line.y2);
                } else if (type == RECTANGLE) {
                    printf("Enter top-left & bottom-right coords (X1 Y1 X2 Y2): ");
                    scanf("%d %d %d %d", &node->data.rect.x1, &node->data.rect.y1, &node->data.rect.x2, &node->data.rect.y2);
                } else if (type == CIRCLE) {
                    printf("Enter center coords & radius (XC YC R): ");
                    scanf("%d %d %d", &node->data.circle.xc, &node->data.circle.yc, &node->data.circle.r);
                } else if (type == TRIANGLE) {
                    printf("Enter three corners (X1 Y1 X2 Y2 X3 Y3): ");
                    scanf("%d %d %d %d %d %d", &node->data.tri.x1, &node->data.tri.y1, &node->data.tri.x2, &node->data.tri.y2, &node->data.tri.x3, &node->data.tri.y3);
                }
                clear_input_buffer();
                add_shape(&head, node);
                printf("[Success] Added Shape ID %d.\n", node->id);
                break;
            }
            case 2: { /* Modify Shape */
                if (!head) { printf("[Info] No shapes on canvas!\n"); break; }
                int mid;
                printf("Enter Shape ID to modify: ");
                if (scanf("%d", &mid) != 1) { clear_input_buffer(); break; }
                clear_input_buffer();

                ShapeNode* target = find_shape(head, mid);
                if (!target) { printf("[Error] Shape ID %d not found!\n", mid); break; }

                if (target->type == LINE) {
                    printf("New coords (X1 Y1 X2 Y2): ");
                    scanf("%d %d %d %d", &target->data.line.x1, &target->data.line.y1, &target->data.line.x2, &target->data.line.y2);
                } else if (target->type == RECTANGLE) {
                    printf("New coords (X1 Y1 X2 Y2): ");
                    scanf("%d %d %d %d", &target->data.rect.x1, &target->data.rect.y1, &target->data.rect.x2, &target->data.rect.y2);
                } else if (target->type == CIRCLE) {
                    printf("New parameters (XC YC R): ");
                    scanf("%d %d %d", &target->data.circle.xc, &target->data.circle.yc, &target->data.circle.r);
                } else if (target->type == TRIANGLE) {
                    printf("New vertices (X1 Y1 X2 Y2 X3 Y3): ");
                    scanf("%d %d %d %d %d %d", &target->data.tri.x1, &target->data.tri.y1, &target->data.tri.x2, &target->data.tri.y2, &target->data.tri.x3, &target->data.tri.y3);
                }
                clear_input_buffer();
                printf("[Success] Shape ID %d updated.\n", mid);
                break;
            }
            case 3: { /* Delete Shape */
                if (!head) { printf("[Info] No active shapes to remove!\n"); break; }
                int did;
                printf("Enter Shape ID to delete: ");
                if (scanf("%d", &did) != 1) { clear_input_buffer(); break; }
                clear_input_buffer();

                if (delete_shape(&head, did)) printf("[Success] Deleted shape %d.\n", did);
                else printf("[Error] Shape %d not found.\n", did);
                break;
            }
            case 4: { /* Save to File */
                char fName[128];
                printf("Enter filename: ");
                scanf("%127s", fName);
                clear_input_buffer();
                save_drawing(head, fName);
                break;
            }
            case 5: { /* Load from File */
                char fName[128];
                printf("Enter filename: ");
                scanf("%127s", fName);
                clear_input_buffer();
                load_drawing(&head, &next_id, fName);
                break;
            }
            case 6: { /* Clear All */
                char confirm;
                printf("Reset everything? (y/n): ");
                scanf("%c", &confirm);
                clear_input_buffer();
                if (confirm == 'y' || confirm == 'Y') {
                    free_all_shapes(&head);
                    next_id = 1;
                    printf("Canvas and structures cleared.\n");
                }
                break;
            }
            default:
                printf("[Error] Select option 1-7.\n");
                break;
        }
    }
    return 0;
}

void reset_canvas(char canvas[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

void plot(char canvas[ROWS][COLS], int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = '*';
}

void render_all_shapes(char canvas[ROWS][COLS], ShapeNode* head) {
    reset_canvas(canvas);
    for (ShapeNode* cur = head; cur != NULL; cur = cur->next) {
        if (cur->type == LINE) draw_line(canvas, cur->data.line.x1, cur->data.line.y1, cur->data.line.x2, cur->data.line.y2);
        else if (cur->type == RECTANGLE) draw_rectangle(canvas, cur->data.rect.x1, cur->data.rect.y1, cur->data.rect.x2, cur->data.rect.y2);
        else if (cur->type == CIRCLE) draw_circle(canvas, cur->data.circle.xc, cur->data.circle.yc, cur->data.circle.r);
        else if (cur->type == TRIANGLE) draw_triangle(canvas, cur->data.tri.x1, cur->data.tri.y1, cur->data.tri.x2, cur->data.tri.y2, cur->data.tri.x3, cur->data.tri.y3);
    }
}

void display_canvas(char canvas[ROWS][COLS]) {
    printf("   +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
    for (int i = 0; i < ROWS; i++) {
        printf("%2d |", i);
        for (int j = 0; j < COLS; j++) printf("%c", canvas[i][j]);
        printf("|\n");
    }
    printf("   +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
}

void draw_line(char canvas[ROWS][COLS], int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy, e2;
    while (1) {
        plot(canvas, x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void draw_rectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    draw_line(canvas, x1, y1, x2, y1);
    draw_line(canvas, x2, y1, x2, y2);
    draw_line(canvas, x2, y2, x1, y2);
    draw_line(canvas, x1, y2, x1, y1);
}

void draw_circle(char canvas[ROWS][COLS], int xc, int yc, int r) {
    if (r < 0) return;
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        plot(canvas, xc + x, yc + y); plot(canvas, xc - x, yc + y);
        plot(canvas, xc + x, yc - y); plot(canvas, xc - x, yc - y);
        plot(canvas, xc + y, yc + x); plot(canvas, xc - y, yc + x);
        plot(canvas, xc + y, yc - x); plot(canvas, xc - y, yc - x);
        if (d > 0) { d += 4 * (x - y) + 10; y--; }
        else d += 4 * x + 6;
        x++;
    }
}

void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}

void add_shape(ShapeNode** head, ShapeNode* new_node) {
    if (*head == NULL) { *head = new_node; return; }
    ShapeNode* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = new_node;
}

int delete_shape(ShapeNode** head, int id) {
    ShapeNode *temp = *head, *prev = NULL;
    if (temp && temp->id == id) { *head = temp->next; free(temp); return 1; }
    while (temp && temp->id != id) { prev = temp; temp = temp->next; }
    if (!temp) return 0;
    prev->next = temp->next;
    free(temp);
    return 1;
}

ShapeNode* find_shape(ShapeNode* head, int id) {
    for (ShapeNode* temp = head; temp; temp = temp->next)
        if (temp->id == id) return temp;
    return NULL;
}

void list_shapes(ShapeNode* head) {
    if (!head) { printf("\nActive Layers: [Empty]\n"); return; }
    printf("\n=== ACTIVE SHAPE LAYERS ===\n");
    for (ShapeNode* temp = head; temp; temp = temp->next) {
        if (temp->type == LINE) printf("ID %d: Line (%d,%d)->(%d,%d)\n", temp->id, temp->data.line.x1, temp->data.line.y1, temp->data.line.x2, temp->data.line.y2);
        else if (temp->type == RECTANGLE) printf("ID %d: Rect (%d,%d) to (%d,%d)\n", temp->id, temp->data.rect.x1, temp->data.rect.y1, temp->data.rect.x2, temp->data.rect.y2);
        else if (temp->type == CIRCLE) printf("ID %d: Circle C(%d,%d) R:%d\n", temp->id, temp->data.circle.xc, temp->data.circle.yc, temp->data.circle.r);
        else if (temp->type == TRIANGLE) printf("ID %d: Tri (%d,%d), (%d,%d), (%d,%d)\n", temp->id, temp->data.tri.x1, temp->data.tri.y1, temp->data.tri.x2, temp->data.tri.y2, temp->data.tri.x3, temp->data.tri.y3);
    }
}

void free_all_shapes(ShapeNode** head) {
    ShapeNode* cur = *head;
    while (cur) {
        ShapeNode* next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

void save_drawing(ShapeNode* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) { printf("[Error] Can't open '%s' for saving!\n", filename); return; }
    for (ShapeNode* t = head; t; t = t->next) {
        fprintf(fp, "%d %d", t->id, (int)t->type);
        if (t->type == LINE) fprintf(fp, " %d %d %d %d\n", t->data.line.x1, t->data.line.y1, t->data.line.x2, t->data.line.y2);
        else if (t->type == RECTANGLE) fprintf(fp, " %d %d %d %d\n", t->data.rect.x1, t->data.rect.y1, t->data.rect.x2, t->data.rect.y2);
        else if (t->type == CIRCLE) fprintf(fp, " %d %d %d\n", t->data.circle.xc, t->data.circle.yc, t->data.circle.r);
        else if (t->type == TRIANGLE) fprintf(fp, " %d %d %d %d %d %d\n", t->data.tri.x1, t->data.tri.y1, t->data.tri.x2, t->data.tri.y2, t->data.tri.x3, t->data.tri.y3);
    }
    fclose(fp);
    printf("[Success] Saved to '%s'.\n", filename);
}

void load_drawing(ShapeNode** head, int* next_id, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) { printf("[Error] Can't read '%s'!\n", filename); return; }
    free_all_shapes(head);
    int id, type, max_id = 0;
    while (fscanf(fp, "%d %d", &id, &type) == 2) {
        ShapeNode* node = (ShapeNode*)malloc(sizeof(ShapeNode));
        if (!node) { fclose(fp); return; }
        node->id = id; node->type = (ShapeType)type; node->next = NULL;
        if (type == LINE) fscanf(fp, "%d %d %d %d", &node->data.line.x1, &node->data.line.y1, &node->data.line.x2, &node->data.line.y2);
        else if (type == RECTANGLE) fscanf(fp, "%d %d %d %d", &node->data.rect.x1, &node->data.rect.y1, &node->data.rect.x2, &node->data.rect.y2);
        else if (type == CIRCLE) fscanf(fp, "%d %d %d", &node->data.circle.xc, &node->data.circle.yc, &node->data.circle.r);
        else if (type == TRIANGLE) fscanf(fp, "%d %d %d %d %d %d", &node->data.tri.x1, &node->data.tri.y1, &node->data.tri.x2, &node->data.tri.y2, &node->data.tri.x3, &node->data.tri.y3);
        if (id > max_id) max_id = id;
        add_shape(head, node);
    }
    fclose(fp);
    *next_id = max_id + 1;
    printf("[Success] Loaded from '%s'.\n", filename);
}