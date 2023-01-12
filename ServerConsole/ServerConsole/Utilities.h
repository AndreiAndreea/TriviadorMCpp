#pragma once
#include <unordered_map>
#include <string>
#include <vector>

enum GameStatus
{
	ROOM_CREATED = 0,
	WAITING_FOR_PLAYERS,
	READY,
	SELECT_QUESTION,
	SINGLE_QUESTION_SELECTED,
	MULTIPLE_QUESTION_SELECTED,
};

const std::string EnumGameStatusToString(GameStatus gameStatus);

// See https://stackoverflow.com/a/57346888/12388382

std::vector<std::string> Split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> ParseUrlArgs(const std::string& urlArgs);