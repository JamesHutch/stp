/********************************************************************
 * AUTHORS: Vijay Ganesh, David L. Dill
 *
 * BEGIN DATE: November, 2005
 *
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/

#include "stp/AST/AST.h"
#include "stp/STPManager/STP.h"

namespace stp
{
const ASTVec ASTBVConst::astbv_empty_children;

ASTBVConst::ASTBVConst(const ASTBVConst& sym)
    : ASTInternal(sym.nodeManager, sym._kind)
{
  _bvconst = CONSTANTBV::BitVector_Clone(sym._bvconst);
  cbv_managed_outside = false;
}

// Call this when deleting a node that has been stored in the the
// unique table
void ASTBVConst::CleanUp()
{
  nodeManager->_bvconst_unique_table.erase(this);
  delete this;
}

// Print function for bvconst -- return _bvconst value in bin
// format (c_friendly is for printing hex. numbers that C
// compilers will accept)
void ASTBVConst::nodeprint(ostream& os, bool c_friendly)
{
  unsigned char* res;
  const char* prefix;

  if (nodeManager->UserFlags.print_binary_flag)
  {
    res = CONSTANTBV::BitVector_to_Bin(_bvconst);
    if (c_friendly)
    {
      prefix = "0b";
    }
    else
    {
      prefix = "0bin";
    }
  }
  else if (getValueWidth() % 4 == 0)
  {
    res = CONSTANTBV::BitVector_to_Hex(_bvconst);
    if (c_friendly)
    {
      prefix = "0x";
    }
    else
    {
      prefix = "0hex";
    }
  }
  else
  {
    res = CONSTANTBV::BitVector_to_Bin(_bvconst);
    if (c_friendly)
    {
      prefix = "0b";
    }
    else
    {
      prefix = "0bin";
    }
  }
  if (NULL == res)
  {
    os << "nodeprint: BVCONST : could not convert to string" << _bvconst;
    FatalError("");
  }
  os << prefix << res;
  CONSTANTBV::BitVector_Dispose(res);
}

CBV ASTBVConst::GetBVConst() const
{
  return _bvconst;
}

size_t ASTBVConst::ASTBVConstHasher::operator()(const ASTBVConst* bvc) const
{
  return CONSTANTBV::BitVector_Hash(bvc->_bvconst);
}

bool ASTBVConst::ASTBVConstEqual::operator()(const ASTBVConst* bvc1,
                                             const ASTBVConst* bvc2) const
{
  if (bvc1->getValueWidth() != bvc2->getValueWidth())
  {
    return false;
  }
  return (0 == CONSTANTBV::BitVector_Compare(bvc1->_bvconst, bvc2->_bvconst));
}

} //end of namespace
