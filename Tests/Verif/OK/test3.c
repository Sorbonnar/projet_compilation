// Expected output:
// a: 28
// b: 28
//

void main() {
    int a, b;
    
    a = 1 + 2 + 3 + 4 + 5 + 6 + 7;
    b = 1 + (2 + (3 + (4 + (5 + (6 + 7)))));

    print("a: ", a, "\nb: ", b);
    print("\n");
}
