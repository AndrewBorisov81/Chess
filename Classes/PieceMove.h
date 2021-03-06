#pragma once

#include "LogicData.h"

#include <variant>

struct SimpleT { Position from, to; int piece; };
struct CaptureT { Position from, to; int piece, captured; bool enpassant; };
struct CastleT { Position king_from, king_to, castle_from, castle_to;
                 bool kingSide; };
struct PromotionT { Position from, to; int typeBefore, typeAfter; };
struct CapturePromotionT { Position from, to; int captured, typeBefore, typeAfter; };
struct IntendedT { Position from, to; int piece; };

struct MoveData
{
  Position from, to;
  Position king_from, king_to, castle_from, castle_to;

  int piece;
  int typeBefore, typeAfter;
  int captured;

  bool enpassant;
  bool kingSide;
};

class PieceMove
{
public:
  std::variant<SimpleT, CaptureT, CastleT, PromotionT, CapturePromotionT, IntendedT> details;

  bool isSimple() const { return details.index() == 0; };
  bool isCapturing() const { return details.index() == 1; };
  bool isCastling() const { return details.index() == 2; };
  bool isPromotion() const { return details.index() == 3; };
  bool isCapturingPromotion() const { return details.index() == 4; };
  bool isIntended() const { return details.index() == 5; };

  void getMoveData(MoveData& moveData);
  void initMoveData(MoveData& moveData);
};
