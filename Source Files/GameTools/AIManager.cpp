#include "AIManager.h"

//BLACK-MAX
//WHITE-MIN

AIManager::AIManager()
{
}


AIManager::~AIManager()
{
}

bool AIManager::isRunning = false;
bool AIManager::started = false;
Movement AIManager::m;

Movement AIManager::randomMoveFor(GameBoard* gb, bool isblack){
	std::vector<Movement> allmoves;
	for (int i = 0; i < gb->height; i++){
		for (int j = 0; j < gb->width; j++){
			if (gb->hasPieceAt(i, j) && gb->getPieceAt(i, j)->isBlack() == isblack){
				auto allmovesforpiece = gb->getPieceAt(i, j)->allPossibleMoves(gb);
				allmoves.insert(allmoves.end(), allmovesforpiece->begin(), allmovesforpiece->end());
			}
		}
	}
	//std::cout << allmoves.size() << " possible moves for " << (isblack ? "black" : "white") << std::endl;
	double bet01 = ((double)rand() / (RAND_MAX));
	int randomm = (int)(bet01 * (allmoves.size() + 1));
	Movement m = allmoves[randomm];
	return m;
}

void AIManager::bestMoveForBlack(GameBoard* gb, int depth){
	clock_t t = clock();
	AIManager::started = true;
	AIManager::isRunning = true;
	std::vector<Movement> allmoves;
	for (int i = 0; i < gb->height; i++){
		for (int j = 0; j < gb->width; j++){
			if (gb->hasPieceAt(i, j) && gb->getPieceAt(i, j)->isBlack() == BLACK){
				auto allmovesforpiece = gb->getPieceAt(i, j)->allPossibleMoves(gb);
				allmoves.insert(allmoves.end(), allmovesforpiece->begin(), allmovesforpiece->end());
			}
		}
	}
	
	int max = INT32_MIN;
	int indexofbestmove = 0;
	for (int i = 0; i < allmoves.size(); i++){
		Movement m = allmoves[i];
		gb->doMove(m);
		int boardscore = minimax(gb, INT32_MIN, INT32_MAX, depth, WHITE);
		gb->popTurn();
		if (boardscore >= max){
			max = boardscore;
			indexofbestmove = i;
		}
	}
	AIManager::isRunning = false;
	Movement m = allmoves[indexofbestmove];
	AIManager::m = m;
	delete(gb);
	t = clock() - t;
	std::cout << ((float)t) / CLOCKS_PER_SEC << std::endl;
}

void AIManager::bestMoveForWhite(GameBoard* gb, int depth){
	clock_t t = clock();
	AIManager::started = true;
	AIManager::isRunning = true;
	std::vector<Movement> allmoves;
	for (int i = 0; i < gb->height; i++){
		for (int j = 0; j < gb->width; j++){
			if (gb->hasPieceAt(i, j) && gb->getPieceAt(i, j)->isBlack() == WHITE){
				auto allmovesforpiece = gb->getPieceAt(i, j)->allPossibleMoves(gb);
				allmoves.insert(allmoves.end(), allmovesforpiece->begin(), allmovesforpiece->end());
			}
		}
	}
	int min = INT32_MAX;
	int indexofbestmove = 0;
	for (int i = 0; i < allmoves.size(); i++){
		Movement m = allmoves[i];
		gb->doMove(m);
		int boardscore = minimax(gb, INT32_MIN, INT32_MAX, depth, BLACK);
		gb->popTurn();
		if (boardscore <= min){
			min = boardscore;
			indexofbestmove = i;
		}
	}
	AIManager::isRunning = false;
	Movement m = allmoves[indexofbestmove];
	AIManager::m= m;
	delete(gb);
	t = clock() - t;
	std::cout << ((float)t) / CLOCKS_PER_SEC << std::endl;
}


int AIManager::minimax(GameBoard* gb, int alphamax, int betamin, int depth, bool currentplayerblack){
	if (depth == 0 || gb->isGameFinished)
		return gb->evaluate();
	std::vector<Movement> allmoves;
	for (int i = 0; i < gb->height; i++){
		for (int j = 0; j < gb->width; j++){
			if (gb->hasPieceAt(i, j) && gb->getPieceAt(i, j)->isBlack() == currentplayerblack){
				auto allmovesforpiece = gb->getPieceAt(i, j)->allPossibleMoves(gb);
				allmoves.insert(allmoves.end(), allmovesforpiece->begin(), allmovesforpiece->end());
			}
		}
	}
	if (currentplayerblack){
		int bestmove = INT32_MIN;
		for (int i = 0; i < allmoves.size(); i++){
			Movement m = allmoves[i];
			gb->doMove(m);
			bestmove = std::max(bestmove, minimax(gb, alphamax, betamin, depth - 1, !currentplayerblack));
			gb->popTurn();
			alphamax = std::max(alphamax, bestmove);
			if (betamin <= alphamax)
				return bestmove;
		}
		return bestmove;
	}
	else{
		int bestmove = INT32_MAX;
		for (int i = 0; i < allmoves.size(); i++){
			Movement m = allmoves[i];
			gb->doMove(m);
			bestmove = std::min(bestmove, minimax(gb, alphamax, betamin, depth - 1, !currentplayerblack));
			gb->popTurn();
			betamin = std::min(betamin, bestmove);
			if (betamin <= alphamax)
				return bestmove;
		}
		return bestmove;
	}
}