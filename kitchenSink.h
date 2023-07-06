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
  size_t size { mbstowcs(nullptr, src, 0) };
  if(size == mbstowcsFailVal) { return L""; }
  std::unique_ptr<wchar_t[]> buffer(new wchar_t[size + 1]);
  size_t ret { mbstowcs(buffer.get(), src, size) };
  if(ret == mbstowcsFailVal) { return L""; }
  buffer.get()[size] = '\0';
  return std::wstring{buffer.get()};
}

template <typename T> static bool vectorContains(std::vector<T> vec, T item)
{ return std::find(vec.begin(), vec.end(), item) != vec.end(); }

template <typename T> static int64_t vectorIndexOf(vector<T> vec, T item)
{
  std::vector<T>::iterator position = std::find(vec.begin(), vec.end(), item);
  return ( position == vec.end() ? -1 : (position - vec.begin()) );
}

static void stringToLower(std::string& str)
{ std::transform( str.begin(), str.end(), str.begin(), [](char c)->char{return tolower(c);} ) }

static void stringTrim(std::string& str)
{
  auto notSpace = [](char c)->bool{ return !std::isspace(c); };
  std::erase( str.begin(), std::find_if(str.begin(), str.end(), notSpace) );
  std::erase( std::find_if(str.rbegin(), str.rend(), notSpace).base(), str.end() );
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
