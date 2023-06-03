// Useful functionality
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <locale>

// iostream, fstream, vector
static bool openFile(const char* filename, std::vector<char>& data) noexcept
{
  std::ifstream file(filename, std::ios_base::binary);
  if(file.fail()) { printf("File Open Fail: %s\n", filename); return false; }
  data = std::vector<char>( (std::istreambuf_iterator<char>(file)) , (std::istreambuf_iterator<char>()) );
  if(data.size() <= 0) { printf ("File Empty\n"); return false; }
  return true;
}

static std::wstring convertCharToWChar(const char* src) noexcept
{
  constexpr size_t mbstowcsFailVal = -1;
  size_t size {mbstowcs(nullptr, src, 0)};
  if(size == mbstowcsFailVal) { return L""; }
  std::unique_ptr<wchar_t[]> buffer(new wchar_t[size + 1]);
  return ( mbstowcs(buffer.get(), src, size) == mbstowcsFailVal ) ? L"" : std::wstring(buffer.get());
}

// locale
// Useful Locale Codes, complex ones are Win, .UTF-8 can be appened to simple ones when setting locale.
// Japanese_Japan.932             ja-JP
// English_United States.1252     en-US
static const char* getUserDefaultConsoleLocale() noexcept { return setlocale(LC_ALL, ""); }
static void setConsoleLocale(std::locale& locale) noexcept
{
  setlocale(LC_ALL, locale.name().c_str());
  std::locale::global(locale);
  std::cout.imbue(locale); // or cerr, clog, wcout, wcerr, wclog
}

class Logging
{
  public:
    enum class LoggingLevel { Debug = -1, Normal, Warning, Error };
    void Log(std::string message, LoggingLevel level = LoggingLevel::Normal)
    { Log(message.c_str(), level); }
    void Log(const char* message, LoggingLevel level = LoggingLevel::Normal)
    {
      switch (level)
      {
      case LoggingLevel::Debug: return write(message, blue);
      case LoggingLevel::Normal: return write(message);
      case LoggingLevel::Warning: return write(message, yellow);
      case LoggingLevel::Error: return write(message, red);
      default: return write("Error: Invalid Log Level\n", blue);
      }
    }
  private:
    void write(const char* msg, const char* color = reset) { printf("%s%s%s", color, msg, reset); }
    constexpr static const char *red = "\e[31m", *yellow = "\e[33m", *blue = "\e[36m", *reset = "\e[0m";
};
