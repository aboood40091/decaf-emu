#include "shader_compiler.h"

unsigned long
parseNumber(peg::Ast &node)
{
   assert(node.is_token);
   return std::stoul(node.token);
}

float
parseFloat(peg::Ast &node)
{
   return std::stof(node.token);
}

uint32_t
parseHexNumber(peg::Ast &node)
{
   return static_cast<uint32_t>(std::stoul(node.token, 0, 0));
}

LiteralValue
parseLiteral(peg::Ast &node)
{
   auto literal = LiteralValue { 0 };

   for (auto child : node.nodes) {
      if (child->name == "HexNumber") {
         literal.flags |= LiteralValue::ReadHex;
         literal.hexValue = parseHexNumber(*child);
      } else if (child->name == "Float") {
         literal.flags |= LiteralValue::ReadFloat;
         literal.floatValue = parseFloat(*child);
      }
   }

   return literal;
}
