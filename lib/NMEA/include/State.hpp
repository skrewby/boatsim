#pragma once

struct State {
    // PGN 129026 - COGSOGRapid
    double cog{0.0};
    double sog{0.0};

    // PGN 130312
    double temperature{0.0};

    // PGN 130313
    double humidity{0.0};

    // PGN 130314
    double pressure{0.0};
};
