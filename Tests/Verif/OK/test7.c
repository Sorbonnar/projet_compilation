int limit = 3;

void main() {
    int i, j, a = 3, b = 4;
    for (i = 0; i <= limit; i = i + 1) {
        for (j = 0; j < limit + 1; j = j + 1) {
            print("(", i, ", ", j, ")\n");
            a = i * 2 + (j - b);
            print("a: ", a, "\n");
        }
    }
    print("a: ", a);
}