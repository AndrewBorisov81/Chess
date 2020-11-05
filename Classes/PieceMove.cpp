#include "PieceMove.h"

void PieceMove::getMoveData(MoveData& moveData)
{

  // init moveData
  initMoveData(moveData);

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
    moveData.captured = captureMove.captured;

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
  else if (isCapturingPromotion())
  {
    CapturePromotionT capturePromotion = std::get<CapturePromotionT>(details);

    moveData.from = capturePromotion.from;
    moveData.to = capturePromotion.to;

    moveData.typeBefore = capturePromotion.typeBefore;
    moveData.typeBefore = capturePromotion.typeAfter;

    moveData.captured = capturePromotion.captured;
  }
  else if (isIntended())
  {
    IntendedT intendedMove = std::get<IntendedT>(details);
    
    moveData.from = intendedMove.from;
    moveData.to = intendedMove.to;

    moveData.piece = intendedMove.piece;
  }
}

void PieceMove::initMoveData(MoveData& moveData)
{
	moveData.piece = 0;
	moveData.typeBefore = 0;
	moveData.typeAfter = 0;
	moveData.captured = 0;

	moveData.enpassant = false;
	moveData.kingSide = false;
}
