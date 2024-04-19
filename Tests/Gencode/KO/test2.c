// Expected output:
// Max: 4294967295
// Min: -4294967295
// Overflow: 4294967296
// Underflow: -4294967296
//
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
