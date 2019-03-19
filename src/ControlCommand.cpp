
#include "ControlCommand.h"

void ControlCommand::read(const QJsonObject &json)
{
    if (json.contains("command") && json["command"].isString())
        command = json["command"].toString();

    if (json.contains("top") && json["top"].isDouble())
        top = json["top"].toDouble();

    if (json.contains("left") && json["left"].isDouble())
        left = json["left"].toDouble();

    if (json.contains("right") && json["right"].isDouble())
        right = json["right"].toDouble();

    if (json.contains("min") && json["min"].isDouble())
        min = json["min"].toDouble();

    if (json.contains("max") && json["max"].isDouble())
        max = json["max"].toDouble();
}