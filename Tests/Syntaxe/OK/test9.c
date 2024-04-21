// Expected output:
// BAND: 0
// BOR: 15
// BXOR: 15
// !C: 0
// -A: -5
// ~A: -6
// Shift Left: 20
// Shift Right: 2
//

void main() {
    int a = 5;
    int b = 10;
    bool c = true;

    int band = a & b;
    int bor = a | b;
    int bxor = a ^ b;

    bool notC = !c;
    int minA = -a;
    int notA = ~a;

    int SL = a << 2;
    int SR = b >> 2;

    print("\nBAND: ", band);
    print("\nBOR: ", bor);
    print("\nBXOR: ", bxor);

    print("\n!C: ", notC);
    print("\n-A: ", minA);
    print("\n~A: ", notA);

    print("\nShift Left: ", SL);
    print("\nShift Right: ", SR);
}
