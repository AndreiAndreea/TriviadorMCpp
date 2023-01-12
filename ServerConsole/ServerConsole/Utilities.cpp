#include "Utilities.h"

const std::string EnumGameStatusToString(GameStatus qStatus)
{
    switch (qStatus)
    {
	    case ROOM_CREATED:
		    return "ROOM_CREATED";

	    case WAITING_FOR_PLAYERS:
		    return "WAITING_FOR_PLAYERS";

	    case READY:
            return "READY";
        
        case SELECT_QUESTION:
            return "SELECT_QUESTION";

        case SINGLE_QUESTION_SELECTED:
            return "SINGLE_QUESTION_SELECTED";

        case MULTIPLE_QUESTION_SELECTED:
            return "MULTIPLE_QUESTION_SELECTED";

        default:
            return "NOT_DEFINED";
    }
}

std::vector<std::string> Split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> result;
    size_t startIndex = 0;

    for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
    {
        result.emplace_back(str.begin() + startIndex, str.begin() + found);
        startIndex = found + delim.size();
    }

    if (startIndex != str.size())
    {
        result.emplace_back(str.begin() + startIndex, str.end());
    }

    return result;
}

std::unordered_map<std::string, std::string> ParseUrlArgs(const std::string& urlArgs)
{
    if (urlArgs == "") {
        return {};
    }

    std::unordered_map<std::string, std::string> result;
    for (const auto& kvStr : Split(urlArgs, "&")) {
        auto kvVector = Split(kvStr, "=");
        if (kvVector.size() == 2)
            result[kvVector[0]] = kvVector[1];
    }
    return result;
}