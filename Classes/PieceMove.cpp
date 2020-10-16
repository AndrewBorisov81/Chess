#include "PieceMove.h"

void PieceMove::getMoveData(MoveData& moveData)
{
  if (isSimple())
  {
    SimpleT simpleMove = std::get<SimpleT>(details);

    moveData.from = simpleMove.from;
    moveData.to = simpleMove.to;

    moveData.piece = simpleMove.piece;
  }
  else if (isCapturing())
  {
    CaptureT captureMove = std::get<CaptureT>(details);
    
    moveData.from = captureMove.from;
    moveData.to = captureMove.to;

    moveData.piece = captureMove.piece;

    moveData.enpassant = captureMove.enpassant;
  }
  else if (isCastling())
  {
    CastleT castlingMove = std::get<CastleT>(details);

    moveData.king_from = castlingMove.king_from;
    moveData.king_to = castlingMove.king_to;

    moveData.castle_from = castlingMove.castle_from;
    moveData.castle_to = castlingMove.castle_to;

    moveData.kingSide = castlingMove.kingSide;
  }
  else if (isPromotion())
  {
    PromotionT promotionMove = std::get<PromotionT>(details);

    moveData.from = promotionMove.from;
    moveData.to = promotionMove.to;

    moveData.typeBefore = promotionMove.typeBefore;
    moveData.typeBefore = promotionMove.typeAfter;
  }
  else if (isIntended())
  {
    IntendedT intendedMove = std::get<IntendedT>(details);
    
    moveData.from = intendedMove.from;
    moveData.to = intendedMove.to;

    moveData.piece = intendedMove.piece;
  }
}