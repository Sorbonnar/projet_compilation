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

    print("BAND: ", band);
    print("BOR: ", bor);
    print("BXOR: ", bxor);

    print("!C: ", notC);
    print("-A: ", minA);
    print("~A: ", notA);

    print("Shift Left: ", SL);
    print("Shift Right: ", SR);
}
