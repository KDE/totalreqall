// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>>
// SPDX-License-Identifier: BSD-2-Clause

#include "UserSettings.h"
#include <QStringList>

namespace TotalReqall
{
    QStringList splitItem(const QString &itemToSplit)
    {
        auto settings = UserSettings::global().data();
        if (settings->splitContent())
        {
            QStringList contentList{ itemToSplit };
            bool allSegmentsProperSize = false;
            do
            {
                for (int i = 0; i < contentList.size(); ++i)
                {
                    if (contentList.at(i).size() > settings->splitThreshold())
                    {
                        if (contentList[i].split("\n\n").size() > 1)
                        {
                            QString sep = "\n\n";
                            QStringList split = contentList.at(i).split(sep);
                            contentList.removeAt(i);
                            for (int j = 0; j < split.size(); ++j)
                            {
                                if (split[j].size() < settings->chunkSize() &&
                                    QString(split[j] + split[j + 1]).size() < settings->chunkSize())
                                {
                                    contentList.insert(i + j, split[j] + sep + split[j + 1]);
                                    split.removeAt(j + 1);
                                }
                                else
                                    contentList.insert(i + j, split[j]);
                            }
                            continue;
                        }

                        // This is not an "else if" because the previous "if" may have split
                        // contentList[i] into a chunk that is still too large, meaning that this
                        // block needs to run as well
                        if (contentList[i].split("\n").size() > 2)
                        {
                            QString sep = "\n";
                            QStringList split = contentList[i].split(sep);
                            contentList.removeAt(i);
                            for (int j = 0; j < split.size() - 1; ++j)
                            {
                                if (split[j].size() < settings->chunkSize() &&
                                    QString(split[j] + split[j + 1]).size() < settings->chunkSize())
                                {
                                    contentList.insert(i + j, split[j] + sep + split[j + 1]);
                                    split.removeAt(j + 1);
                                }
                                else
                                    contentList.insert(i + j, split[j]);
                            }
                            continue;
                        }
                        else
                            allSegmentsProperSize = true; // this couldn't be split
                    }
                }
                allSegmentsProperSize = true;
                for (auto i : contentList)
                    allSegmentsProperSize &= (i.size() < settings->splitThreshold() ||
                                              i.size() < (2 * settings->chunkSize()));
            } while (!allSegmentsProperSize);

            return contentList;
        }
        else
            return QStringList{ itemToSplit };
    }
} // namespace TotalReqall
