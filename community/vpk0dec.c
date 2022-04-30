/*****************************************************
 *  vpk0 Decode Function Instance #2 [FUN_80103530]  *
 *  Decompiled from Pokémon Snap [NPFE]              *
 *  using                                            *
 *  Ghidra 9.1.2                                     *
 *  and                                              *
 *  N64 Loader by Warranty Voider                    *
 *****************************************************/
#include <stdint.h>
#include <stdbool.h>



/*====================================================================
    [param_1] is the topmost signed address of the encoded source.
    [param_2] is the pointer to the decoded unsigned byte buffer.

    Note:   vpk0 data is Big Endian.
            This routine isn't rigged for Intel/AMD host usage.
            Instead, this exists for those who wish to use vpk0
            data in a Nintendo 64 homebrew project encoded with
            CaitSith2's "nvpktool".
====================================================================*/
void decode_vpk0( int32_t param_1, uint8_t *param_2 )
{
  uint8_t uVar1;
  uint16_t uVar2;
  int32_t iVar3;
  int32_t iVar4;
  uint32_t uVar5;
  uint16_t *puVar6;
  int32_t **ppiVar7;
  int32_t iVar8;
  int32_t iVar9;
  uint8_t *puVar10;
  int32_t *piVar11;
  uint32_t uVar12;
  uint8_t *puVar13;
  int32_t **local_3dc [24];
  int32_t **local_37c [22];
  uint32_t local_324;
  int32_t *local_30c [195];
  
  puVar10 = param_2 + *(int32_t *)(param_1 + 4);
  iVar3 = 8;
  uVar5 = *(int32_t *)(param_1 + 4) << 0x10 | (uint32_t)*(uint16_t *)(param_1 + 8);
  local_324 = ((uint32_t)*(uint16_t *)(param_1 + 8) << 0x10) >> 0x18;
  puVar6 = (uint16_t *)(param_1 + 10);
  local_37c[2] = (int32_t **)0x0;
  ppiVar7 = local_30c;
  iVar8 = 0;

  while ( true )
  {
    iVar4 = iVar3;

    if ( iVar3 < 1 )
    {
      uVar2 = *puVar6;
      puVar6 = puVar6 + 1;
      iVar4 = iVar3 + 0x10;
      uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
    }

    iVar3 = iVar4 + -1;
    iVar9 = uVar5 << (0x1fU - iVar3 & 0x1f);

    if ( (iVar9 < 0) && (iVar8 < 2) ) break;

    if ( iVar9 < 0 )
    {
      *ppiVar7 = (int32_t *)0x0;
      ppiVar7[1] = (int32_t *)0x0;
      ppiVar7[2] = (int32_t *)0x0;
      *(int32_t ***)ppiVar7 = local_37c[iVar8];
      *(int32_t ***)(ppiVar7 + 1) = local_37c[iVar8 + 1];
      local_37c[iVar8] = ppiVar7;
      ppiVar7 = ppiVar7 + 3;
      iVar8 = iVar8 + -1;
    }
    else
    {
      *ppiVar7 = (int32_t *)0x0;
      ppiVar7[1] = (int32_t *)0x0;
      ppiVar7[2] = (int32_t *)0x0;

      if ( iVar3 < 8 )
      {
        uVar2 = *puVar6;
        puVar6 = puVar6 + 1;
        iVar3 = iVar4 + 0xf;
        uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
      }

      iVar3 = iVar3 + -8;
      ppiVar7[2] = (int32_t *)((uVar5 << (0x18U - iVar3 & 0x1f)) >> 0x18);
      local_37c[iVar8 + 2] = ppiVar7;
      iVar8 = iVar8 + 1;
      ppiVar7 = ppiVar7 + 3;
    }
  }

  local_3dc[2] = (int32_t **)0x0;
  iVar8 = 0;

  while ( true )
  {
    iVar4 = iVar3;

    if ( iVar3 < 1 )
    {
      uVar2 = *puVar6;
      puVar6 = puVar6 + 1;
      iVar4 = iVar3 + 0x10;
      uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
    }

    iVar3 = iVar4 + -1;
    iVar9 = uVar5 << (0x1fU - iVar3 & 0x1f);

    if ( (iVar9 < 0) && (iVar8 < 2) ) break;

    if ( iVar9 < 0 )
    {
      *ppiVar7 = (int32_t *)0x0;
      ppiVar7[1] = (int32_t *)0x0;
      ppiVar7[2] = (int32_t *)0x0;
      *(int32_t ***)ppiVar7 = local_3dc[iVar8];
      *(int32_t ***)(ppiVar7 + 1) = local_3dc[iVar8 + 1];
      local_3dc[iVar8] = ppiVar7;
      ppiVar7 = ppiVar7 + 3;
      iVar8 = iVar8 + -1;
    }
    else
    {
      *ppiVar7 = (int32_t *)0x0;
      ppiVar7[1] = (int32_t *)0x0;
      ppiVar7[2] = (int32_t *)0x0;

      if ( iVar3 < 8 )
      {
        uVar2 = *puVar6;
        puVar6 = puVar6 + 1;
        iVar3 = iVar4 + 0xf;
        uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
      }

      iVar3 = iVar3 + -8;
      ppiVar7[2] = (int32_t *)((uVar5 << (0x18U - iVar3 & 0x1f)) >> 0x18);
      local_3dc[iVar8 + 2] = ppiVar7;
      iVar8 = iVar8 + 1;
      ppiVar7 = ppiVar7 + 3;
    }
  }

  if ( param_2 < puVar10 )
  {
    do
    {
      if ( iVar3 < 1 )
      {
        uVar2 = *puVar6;
        puVar6 = puVar6 + 1;
        uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
        iVar3 = iVar3 + 0xf;
      }
      else
      {
        iVar3 = iVar3 + -1;
      }

      if ( (int32_t)(uVar5 << (0x1fU - iVar3 & 0x1f)) < 0 )
      {
        ppiVar7 = local_37c[2];
        piVar11 = *ppiVar7;

        if ( local_324 == 0 )
        {
          while ( piVar11 != (int32_t *)0x0 )
          {
            if ( iVar3 < 1 )
            {
              uVar2 = *puVar6;
              puVar6 = puVar6 + 1;
              uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
              iVar3 = iVar3 + 0xf;
            }
            else
            {
              iVar3 = iVar3 + -1;
            }

            if ( (int32_t)(uVar5 << (0x1fU - iVar3 & 0x1f)) < 0 )
            {
              ppiVar7 = (int32_t **)ppiVar7[1];
            }
            else
            {
              ppiVar7 = (int32_t **)*ppiVar7;
            }

            piVar11 = *ppiVar7;
          }

          piVar11 = ppiVar7[2];

          if ( iVar3 < (int32_t)piVar11 )
          {
            uVar2 = *puVar6;
            puVar6 = puVar6 + 1;
            iVar3 = iVar3 + 0x10;
            uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
          }

          iVar3 = iVar3 - (int32_t)piVar11;
          iVar8 = -((uVar5 << ((0x20 - (int32_t)piVar11) - iVar3 & 0x1fU)) >> (0x20U - (int32_t)piVar11 & 0x1f));
        }
        else
        {
          iVar8 = 0;

          while ( piVar11 != (int32_t *)0x0 )
          {
            if ( iVar3 < 1 )
            {
              uVar2 = *puVar6;
              puVar6 = puVar6 + 1;
              uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
              iVar3 = iVar3 + 0xf;
            }
            else
            {
              iVar3 = iVar3 + -1;
            }

            if ( (int32_t)(uVar5 << (0x1fU - iVar3 & 0x1f)) < 0 )
            {
              ppiVar7 = (int32_t **)ppiVar7[1];
            }
            else
            {
              ppiVar7 = (int32_t **)*ppiVar7;
            }

            piVar11 = *ppiVar7;
          }

          piVar11 = ppiVar7[2];

          if ( iVar3 < (int32_t)piVar11 )
          {
            uVar2 = *puVar6;
            puVar6 = puVar6 + 1;
            iVar3 = iVar3 + 0x10;
            uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
          }

          iVar3 = iVar3 - (int32_t)piVar11;
          uVar12 = (uVar5 << ((0x20 - (int32_t)piVar11) - iVar3 & 0x1fU)) >> (0x20U - (int32_t)piVar11 & 0x1f);

          if ( (int32_t)uVar12 < 3 )
          {
            iVar8 = uVar12 + 1;
            ppiVar7 = local_37c[2];
            piVar11 = *ppiVar7;

            while ( piVar11 != (int32_t *)0x0 )
            {
              if ( iVar3 < 1 )
              {
                uVar2 = *puVar6;
                puVar6 = puVar6 + 1;
                uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
                iVar3 = iVar3 + 0xf;
              }
              else
              {
                iVar3 = iVar3 + -1;
              }

              if ( (int32_t)(uVar5 << (0x1fU - iVar3 & 0x1f)) < 0 )
              {
                ppiVar7 = (int32_t **)ppiVar7[1];
              }
              else
              {
                ppiVar7 = (int32_t **)*ppiVar7;
              }

              piVar11 = *ppiVar7;
            }

            piVar11 = ppiVar7[2];

            if ( iVar3 < (int32_t)piVar11 )
            {
              uVar2 = *puVar6;
              puVar6 = puVar6 + 1;
              iVar3 = iVar3 + 0x10;
              uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
            }

            iVar3 = iVar3 - (int32_t)piVar11;
            uVar12 = (uVar5 << ((0x20 - (int32_t)piVar11) - iVar3 & 0x1fU)) >> (0x20U - (int32_t)piVar11 & 0x1f);
          }

          iVar8 = (uVar12 * -4 - iVar8) + 8;
        }

        puVar13 = param_2 + iVar8;
        ppiVar7 = local_3dc[2];
        iVar8 = (int32_t)ppiVar7[2];

        if ( *ppiVar7 != (int32_t *)0x0 )
        {
          do
          {
            if ( iVar3 < 1 )
            {
              uVar2 = *puVar6;
              puVar6 = puVar6 + 1;
              uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
              iVar3 = iVar3 + 0xf;
            }
            else
            {
              iVar3 = iVar3 + -1;
            }

            if ( (int32_t)(uVar5 << (0x1fU - iVar3 & 0x1f)) < 0 )
            {
              ppiVar7 = (int32_t **)ppiVar7[1];
            }
            else
            {
              ppiVar7 = (int32_t **)*ppiVar7;
            }
          }
          while ( *ppiVar7 != (int32_t *)0x0 );

          iVar8 = (int32_t)ppiVar7[2];
        }

        if ( iVar3 < iVar8 )
        {
          uVar2 = *puVar6;
          puVar6 = puVar6 + 1;
          iVar3 = iVar3 + 0x10;
          uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
        }

        iVar3 = iVar3 - iVar8;
        uVar12 = (uVar5 << ((0x20 - iVar8) - iVar3 & 0x1fU)) >> (0x20U - iVar8 & 0x1f);

        while ( 0 < (int32_t)uVar12 )
        {
          uVar12 = uVar12 - 1;
          uVar1 = *puVar13;
          puVar13 = puVar13 + 1;
          *param_2 = uVar1;
          param_2 = param_2 + 1;
        }
      }
      else
      {
        if ( iVar3 < 8 )
        {
          uVar2 = *puVar6;
          puVar6 = puVar6 + 1;
          uVar5 = uVar5 << 0x10 | (uint32_t)uVar2;
          iVar3 = iVar3 + 8;
        }
        else
        {
          iVar3 = iVar3 + -8;
        }

        *param_2 = (char)((uVar5 << (0x18U - iVar3 & 0x1f)) >> 0x18);
        param_2 = param_2 + 1;
      }
    }
    while ( param_2 < puVar10 );
  }

  return;
}
