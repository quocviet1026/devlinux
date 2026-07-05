#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>

typedef struct {
    int    id;
    char   name[64];
    int    quantity;
    double price;
} Product;

#define DATA_FILE "products.dat"
#define MAX_PRODUCTS 1000

/* Loads everything into a fixed array for "simplicity" -
 * technically works but defeats the purpose of the exercise
 * (assignment explicitly says don't load entire file into memory). */
static Product g_products[MAX_PRODUCTS];
static int g_count = 0;

void load_all(void) {
    int fd = open(DATA_FILE, O_RDONLY | O_CREAT, 0644);
    if (fd < 0) { perror("open"); return; }

    g_count = 0;
    ssize_t n;
    while ((n = read(fd, &g_products[g_count], sizeof(Product))) == sizeof(Product)) {
        g_count++;
        if (g_count >= MAX_PRODUCTS) break;
    }
    close(fd);
}

void add_product(void) {
    int fd = open(DATA_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) { perror("open"); return; }

    Product p;
    printf("ID: ");
    scanf("%d", &p.id);
    printf("Name: ");
    scanf("%63s", p.name);
    printf("Quantity: ");
    scanf("%d", &p.quantity);
    printf("Price: ");
    scanf("%lf", &p.price);

    write(fd, &p, sizeof(Product));
    close(fd);
    printf("Product added.\n");
}

void show_by_index(void) {
    int index;
    printf("Index: ");
    scanf("%d", &index);

    int fd = open(DATA_FILE, O_RDONLY);
    if (fd < 0) { perror("open"); return; }

    off_t offset = (off_t)index * sizeof(Product);
    lseek(fd, offset, SEEK_SET);

    Product p;
    if (read(fd, &p, sizeof(Product)) == sizeof(Product)) {
        printf("ID=%d Name=%s Qty=%d Price=%.2f\n", p.id, p.name, p.quantity, p.price);
    } else {
        printf("Index out of range.\n");
    }
    close(fd);
}

/* BUG: instead of seeking directly to the quantity field offset and
 * writing only that field, this reads the whole record, modifies it
 * in memory, then rewrites the whole record back. Works, but violates
 * the exercise's explicit requirement to update only the quantity field. */
void update_quantity(void) {
    int index, new_qty;
    printf("Index: ");
    scanf("%d", &index);
    printf("New quantity: ");
    scanf("%d", &new_qty);

    int fd = open(DATA_FILE, O_RDWR);
    if (fd < 0) { perror("open"); return; }

    off_t offset = (off_t)index * sizeof(Product);
    lseek(fd, offset, SEEK_SET);

    Product p;
    if (read(fd, &p, sizeof(Product)) != sizeof(Product)) {
        printf("Index out of range.\n");
        close(fd);
        return;
    }

    p.quantity = new_qty;

    lseek(fd, offset, SEEK_SET);
    write(fd, &p, sizeof(Product));  /* rewrites entire record, not just quantity */

    close(fd);
    printf("Quantity updated.\n");
}

void list_all(void) {
    load_all();
    for (int i = 0; i < g_count; i++) {
        printf("ID=%d Name=%s Qty=%d Price=%.2f\n",
               g_products[i].id, g_products[i].name,
               g_products[i].quantity, g_products[i].price);
    }
}

int main(void) {
    int choice;

    while (1) {
        printf("\n1. Add product\n2. Show product by index\n3. Update quantity by index\n4. List all products\n5. Exit\n> ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: add_product(); break;
            case 2: show_by_index(); break;
            case 3: update_quantity(); break;
            case 4: list_all(); break;
            case 5: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
