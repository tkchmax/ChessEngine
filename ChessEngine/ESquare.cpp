#include "ESquare.h"
#include <sstream>
#include <cassert>

//const char* ToString(ESquare square)
//{
//	assert((int)square > 0 && (int)square < 64);
//	char rank = char(((int)square / 8) + '1');
//	char file = char((int)square % 8 + 'a');
//
//	std::stringstream ss;
//	ss << file << rank;
//
//	std::string coord;
//	ss >> coord;
//	
//	return coord.c_str();
//}

const char* ToString(ESquare square)
{
    assert((int)square >= 0 && (int)square < 64);
    switch (square)
    {
    case ESquare::A1: return "a1";
    case ESquare::A2: return "a2";
    case ESquare::A3: return "a3";
    case ESquare::A4: return "a4";
    case ESquare::A5: return "a5";
    case ESquare::A6: return "a6";
    case ESquare::A7: return "a7";
    case ESquare::A8: return "a8";

    case ESquare::B1: return "b1";
    case ESquare::B2: return "b2";
    case ESquare::B3: return "b3";
    case ESquare::B4: return "b4";
    case ESquare::B5: return "b5";
    case ESquare::B6: return "b6";
    case ESquare::B7: return "b7";
    case ESquare::B8: return "b8";

    case ESquare::C1: return "c1";
    case ESquare::C2: return "c2";
    case ESquare::C3: return "c3";
    case ESquare::C4: return "c4";
    case ESquare::C5: return "c5";
    case ESquare::C6: return "c6";
    case ESquare::C7: return "c7";
    case ESquare::C8: return "c8";

    case ESquare::D1: return "d1";
    case ESquare::D2: return "d2";
    case ESquare::D3: return "d3";
    case ESquare::D4: return "d4";
    case ESquare::D5: return "d5";
    case ESquare::D6: return "d6";
    case ESquare::D7: return "d7";
    case ESquare::D8: return "d8";

    case ESquare::E1: return "e1";
    case ESquare::E2: return "e2";
    case ESquare::E3: return "e3";
    case ESquare::E4: return "e4";
    case ESquare::E5: return "e5";
    case ESquare::E6: return "e6";
    case ESquare::E7: return "e7";
    case ESquare::E8: return "e8";

    case ESquare::F1: return "f1";
    case ESquare::F2: return "f2";
    case ESquare::F3: return "f3";
    case ESquare::F4: return "f4";
    case ESquare::F5: return "f5";
    case ESquare::F6: return "f6";
    case ESquare::F7: return "f7";
    case ESquare::F8: return "f8";

    case ESquare::G1: return "g1";
    case ESquare::G2: return "g2";
    case ESquare::G3: return "g3";
    case ESquare::G4: return "g4";
    case ESquare::G5: return "g5";
    case ESquare::G6: return "g6";
    case ESquare::G7: return "g7";
    case ESquare::G8: return "g8";

    case ESquare::H1: return "h1";
    case ESquare::H2: return "h2";
    case ESquare::H3: return "h3";
    case ESquare::H4: return "h4";
    case ESquare::H5: return "h5";
    case ESquare::H6: return "h6";
    case ESquare::H7: return "h7";
    case ESquare::H8: return "h8";
    }

}

