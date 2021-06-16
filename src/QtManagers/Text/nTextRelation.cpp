#include <qtmanagers.h>

N::TextRelation:: TextRelation (void         )
                : Object       (0,Types::None)
                , relation     (0            )
{
}

N::TextRelation:: TextRelation (SUID uuid,int type,int relate)
                : Object       (     uuid,    type           )
                , relation     (                       relate)
{
}

N::TextRelation:: TextRelation (const TextRelation & relate)
{
  ME = relate ;
}

N::TextRelation::~TextRelation (void)
{
}

N::TextRelation & N::TextRelation::operator = (const TextRelation & relate)
{
  nMemberCopy ( relate , uuid      ) ;
  nMemberCopy ( relate , type      ) ;
  nMemberCopy ( relate , relation  ) ;
  nMemberCopy ( relate , arguments ) ;
  return ME                          ;
}
