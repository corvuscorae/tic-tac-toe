#include "Logger.h"

Logger* Logger::instance = nullptr;

void Logger::ToggleConsoleLog(bool b)
{
    this->to_console_enabled = b;
}

void Logger::WriteLogToFile(const std::string &_filename, bool show_info, bool show_warn, bool show_error)
{
    // OPEN FILE
    if (this->file.is_open())
    {
        this->file.close();
    }

    this->filename = _filename;
    this->file.open(this->filename, std::ios::out);

    // WRITE
    if (this->file.is_open())
    {
        for (int i = 0; i < this->log.size(); i++)
        {
            LogItem item = this->log.at(i);

            // skip deselected levels
            if(!show_info && item.level == level_text[INFO]){
                continue;
            }
            if(!show_warn && item.level == level_text[WARN]){
                continue;
            }
            if(!show_error && item.level == level_text[ERROR]){
                continue;
            }
            this->file << item.print() << "\n";
        }
        this->file.flush();
    }
}

void Logger::Log(const char *message, int lvl, int type){
    LogItem new_item(level_text[lvl], message, color[lvl], type_text[type]);
    this->log.push_back(new_item);

    if (this->to_console_enabled)
    {
        std::cout << new_item.print() << std::endl;
    }

    this->log_size++;
}

void Logger::clear()
{
    this->log.clear();
    this->log_size = 0;
}

LogItem Logger::get(int i)
{
    return this->log.at(i);
}

std::string Logger::print_last()
{
    return this->log.back().print();
}

std::string Logger::print(int i)
{
    return this->log.at(i).print();
}
