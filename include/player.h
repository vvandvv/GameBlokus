#pragma once

#include "game_board.h"
#include "chessbox.h"
#include "message.h"

class Player {
	ChessBox *const chess_box = new ChessBox();
	const int player_id;
	const Point birth_place;
public:
	GameBoard *mGameBoard;
public:
	Player(int pid, GameBoard *gb, const Point &pt) : player_id(pid), mGameBoard(gb), birth_place(pt) {}
	~Player() {
		delete chess_box;
	}
public:
	//��ҵõ���ͼ�����п����ߵ�λ��
	Points getAvailablePoints() const {
		Points res;
		if (chess_box->getChessNum() == ConstDefs::CHESSMAN_TOTAL) {
			res.push_back(birth_place);
		} else {
			res = mGameBoard->getAvailablePoints(player_id);
		}
		return res;
	}
	//���ѡ��һ�����ӷ�����������
	void putChess() const {
		Points pos = getAvailablePoints();
		size_t posl;
		Chessman::ShapeSet fs;
		for (ChessBox::ChessIter chess_iter = chess_box->begin(); chess_iter != chess_box->end(); ) {
			size_t plen = pos.size();
			size_t i;
			for (i = 0; i < plen; ++i) {
				fs = mGameBoard->getValidShapes(player_id, *chess_iter, pos[i].x, pos[i].y);
				if (!fs.empty()) {
					break;
				}
			}
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			mGameBoard->putChessAt(player_id, *chess_iter, fs.begin(), pos[i].x, pos[i].y);
			chess_box->removeChess(chess_iter);
			mGameBoard->showInScreen();
			printf("\n");
			break;
		}
	}
	MsgAction *getAction(const MsgInquire *iqr) const {
		Points pos = getAvailablePoints();
		Chessman::ShapeSet fs;
		//�������е�����
		for (ChessBox::ChessIter chess_iter = chess_box->begin(); chess_iter != chess_box->end(); ) {
			//�������пɷ��õ�λ��
			for (const Point &ips : pos) {
				//�õ��ڸ�λ�����п��ܵ���״
				fs = mGameBoard->getValidShapes(player_id, *chess_iter, ips.x, ips.y);
				//����ҵ����Էŵ���״���Ͳ���Ѱ��
				if (!fs.empty()) {
					break;
				}
				//������λ�ò��ܷŸ����ӣ���ô������һ��λ��
			}
			//���û���κ�λ���ܷ��ø����ӣ��ͼ�����һ������
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			//����ҵ���������ܹ��ŵĵ�һ��λ�ã���ȡ���пɷ�����״�ĵ�һ��
			(*chess_iter)->shape = *fs.begin();
			return new MsgAction(iqr, *chess_iter);
		}
	}
};