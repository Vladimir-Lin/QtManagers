#include <qtmanagers.h>

N::SubgroupPictures:: SubgroupPictures (QWidget * parent,Plan * p)
                    : SubgroupLists    (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::SubgroupPictures::~SubgroupPictures(void)
{
}

void N::SubgroupPictures::Configure(void)
{
}

void N::SubgroupPictures::DeletePictures(QListWidgetItem * it)
{
  SUID u = nListUuid(it)                         ;
  QString M                                      ;
  M = tr("Delete [%1] pictures").arg(it->text()) ;
  plan->showMessage(M)                           ;
  setEnabled ( false          )                  ;
  EnterSQL   ( SC , plan->sql )                  ;
    QString Q                                    ;
    Q = SC.sql.DeleteFrom                        (
          PlanTable(Groups)                      ,
          FirstItem                              (
            u                                    ,
            EachType()                           ,
            Types::Picture                       ,
            Groups::Subordination                )
        )                                        ;
    SC.Query(Q)                                  ;
  LeaveSQL   ( SC , plan->sql )                  ;
  setEnabled ( true           )                  ;
  Alert      ( Done           )                  ;
}

bool N::SubgroupPictures::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                         ;
  QMenu           * me                              ;
  QMenu           * mc                              ;
  QAction         * aa                              ;
  QListWidgetItem * it                              ;
  SUID              u = 0                           ;
  ///////////////////////////////////////////////////
  it = itemAt(pos)                                  ;
  if (NotNull(it) && !it->isSelected()) it = NULL   ;
  if (NotNull(it)) u = nListUuid(it)                ;
  ///////////////////////////////////////////////////
  mm.add(101,tr("New subgroup"))                    ;
  mm.add(201,tr("Reflush"          ))               ;
  if (NotNull(it)) mm.add(202,tr("Reflush item"))   ;
  mm.addSeparator()                                 ;
  me = mm.addMenu(tr("Edit"      ))                 ;
  if (NotNull(it))                                  {
    mc = mm.addMenu(tr("Categorize"))               ;
  }                                                 ;
  mm.addSeparator()                                 ;
  mm.add(301,tr("Copy to clipboard"))               ;
  mm.add(302,tr("Clear selection"  ))               ;
  mm.add(303,tr("Select all"       ))               ;
  ///////////////////////////////////////////////////
  if (NotNull(it))                                  {
    mm.add(me,102,tr("Rename"))                     ;
    mm.add(me,103,tr("Delete this subgroup"))       ;
    mm.addSeparator()                               ;
    mm.add(me,104,tr("Delete all pictures"))        ;
    mm.addSeparator()                               ;
    if (u>0) mm.add(me,401,tr("Pictures"))          ;
  }                                                 ;
  mm.add(me,211,tr("Multilingual translations"))    ;
  AdjustMenu(mm,me)                                 ;
  ///////////////////////////////////////////////////
  if (NotNull(it))                                  {
    mm.add(mc,501,tr("Constraints"))                ;
    mm.add(mc,502,tr("Rule tables"))                ;
  }                                                 ;
  ///////////////////////////////////////////////////
  mm . setFont ( plan                )              ;
  aa = mm.exec (                     )              ;
  nKickOut     ( IsNull(aa) , true   )              ;
  ///////////////////////////////////////////////////
  UUIDs Tuids                                       ;
  switch (mm[aa])                                   {
    case 101                                        :
      New             (    )                        ;
    break                                           ;
    case 102                                        :
      Rename          (    )                        ;
    break                                           ;
    case 103                                        :
      Delete          (    )                        ;
    break                                           ;
    case 104                                        :
      DeletePictures  ( it )                        ;
    break                                           ;
    case 201                                        :
      startup         (    )                        ;
    break                                           ;
    case 202                                        :
      Refresh         ( it )                        ;
    break                                           ;
    case 211                                        :
      Tuids = ItemUuids()                           ;
      emit Translations(windowTitle(),Tuids)        ;
    break                                           ;
    case 301                                        :
      CopyToClipboard ( )                           ;
    break                                           ;
    case 302                                        :
      SelectNone      ( )                           ;
    break                                           ;
    case 303                                        :
      SelectAll       ( )                           ;
    break                                           ;
    case 401                                        :
      if (u>0)                                      {
        emit SeePictures(ObjectUuid(),u,it->text()) ;
      }                                             ;
    break                                           ;
    case 501                                        :
      emit Constraints ( windowTitle() , u    )     ;
    break                                           ;
    case 502                                        :
      emit RuleTables  ( windowTitle() , u    )     ;
    break                                           ;
    default                                         :
      RunAdjustment    ( mm            , aa   )     ;
    break                                           ;
  }                                                 ;
  return true                                       ;
}
