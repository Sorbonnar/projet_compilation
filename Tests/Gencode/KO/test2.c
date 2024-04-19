// Overflow and underflow
void main() {
    int max = 0xFFFFFFFF;
    int min = -0xFFFFFFFF;
    int overflow = max + 1;
    int underflow = min - 1;

    print("Max: ", max, "\n");
    print("Min: ", min, "\n");
    print("Overflow: ", overflow, "\n");
    print("Underflow: ", underflow, "\n");
}
