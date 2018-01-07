/* alice: C++ command shell library
 * Copyright (C) 2017-2018  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <alice/alice.hpp>

#include <fmt/format.h>
#include <string>
#include <vector>

namespace alice
{

ALICE_ADD_STORE( std::string, "str", "s", "string", "strings" )
ALICE_ADD_STORE( int, "number", "d", "number", "numbers" )

ALICE_DESCRIBE_STORE( std::string, element )
{
  return fmt::format( "{} characters", element.size() );
}

ALICE_PRINT_STORE( std::string, os, element )
{
  os << element << std::endl;
}

ALICE_DESCRIBE_STORE( int, element )
{
  return element < 10 ? "small number" : "large number";
}

ALICE_PRINT_STORE( int, os, element )
{
  os << element << std::endl;
}

ALICE_CONVERT( int, element, std::string )
{
  if ( element >= 0 && element < 10 )
  {
    return std::vector<std::string>{"zero", "one", "two", "three",
                                    "four", "five", "six", "seven", 
                                    "eight", "nine"}[element];
  }
  else
  {
    return "many";
  }
}

ALICE_SHOW( std::string, "svg", os, element )
{
  const auto svg = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
  <text x="0" y="36" font-size="36" font-family="Verdana">{}</text>
</svg>
  )svg";

  os << fmt::format( svg, element );
}

template<>
bool can_show<int>( std::string& extension, command& cmd )
{
  extension = "ps";

  cmd.add_option<unsigned>( "--fontsize", "font size" );

  return true;
}

template<>
void show<int>( std::ostream& os, const int& element, const command& cmd )
{
  const auto ps = R"ps(
%!PS
/inch {{72 mul}} def

/Times-Roman findfont {} scalefont setfont
2.5 inch 5 inch moveto
({}) show

showpage
  )ps";

  const auto fontsize = cmd.option_value<unsigned>( "--fontsize", 30 );
  os << fmt::format( ps, fontsize, element );
}

class number_command : public command
{
public:
  number_command( const environment::ptr& env )
      : command( env, "reads a number" )
  {
    opts.add_option( "--load,load", number, "number to load to the store" )->required();
  }

protected:
  void execute()
  {
    env->store<int>().extend() = number;
  }

private:
  int number;
};

ALICE_ADD_COMMAND( number, "Generation" )
}

ALICE_MAIN( tutorial5 )