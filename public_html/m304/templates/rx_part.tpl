<tr>
  <th rowspan="2">No.</th>
  <th width="400px">反映時間帯</th>
  <th>リレー状態設定 (1,2,3,4,5,6,7,8)</th>
</tr>
<tr>
  <th colspan="3">複合条件</th>
</tr>
{for $idx=0 to 29}
<tr>
  <td rowspan="2">{$idx}</td>
  <td>
    <input type="CHECKBOX" name="RX_VALID[{$idx}]" {$RX_CHECKED[$idx]}>
    <input type="NUMBER" name="RX_STHR[{$idx}]" class="txt3" min="0" max="23" size="2" maxlength="2" value="{$RXSTHR[$idx]}">:
    <input type="NUMBER" name="RX_STMN[{$idx}]" class="txt3" min="0" max="59" size="2" maxlength="2" value="{$RXSTMN[$idx]}">&#126;
    <input type="NUMBER" name="RX_EDHR[{$idx}]" class="txt3" min="0" max="23" size="2" maxlength="2" value="{$RXEDHR[$idx]}">:
    <input type="NUMBER" name="RX_EDMN[{$idx}]" class="txt3" min="0" max="59" size="2" maxlength="2" value="{$RXEDMN[$idx]}">
    &nbsp;
    <input type="NUMBER" name="RX_INMN[{$idx}]" class="txt3" min="0" max="254" size="3" maxlength="3" value="{$RXINMN[$idx]}">分間隔
    <input type="NUMBER" name="RX_DUMN[{$idx}]" class="txt3" min="0" max="254" size="3" maxlength="3" value="{$RXDUMN[$idx]}">分間
  </td>
  <td align="center">
    {for $rn=1 to 8}
    <select name="RX_RLY{$rn}[{$idx}]">
      {foreach $RLY as $rly}
      {html_options values=$rly.v output=$rly.name selected=$RLYCODE{$rn}[$idx]}
      {/foreach}
    </select>
    {/for}
  </td>
</tr>
<tr>
  <td colspan="3">
    {for $i=0 to 4}
   (<select name="RX_CCMTYPE[{$i}][{$idx}]">
      {foreach $CMPFUNC as $cf}
      {html_options values=$cf.f output=$cf.name selected=$CMPFUNCSEL0[$idx]}
      {/foreach}
    </select>
    <select name="RX_CMP[{$i}][{$idx}]">
      {foreach $CMPOPE as $cope}
      {html_options values=$cope.ope output=$cope.name selected=$OPEMETHOD[$idx].cmpsel}
      {/foreach}
    </select>
    <input type="NUMBER" name="RX_FVAL[{$i}][{$idx}]" class="txt5" size="6" maxlength="6" step="0.1" value="{$RX_FVAL[$i][$idx]}">)
    {if $i < 4}
    <select name="RX_CMBCMP[{$i}][{$idx}]">
      {foreach $FUNCOPE as $fo}
      {html_options values=$fo.f output=$fo.name selected=$FUNCOPESEL{$i}[$idx]}
      {/foreach}
    </select>
    {/if}
    {/for}
  </td>
</tr>
{/for}
{if $ihexrx!="NON"}
<tr>
  <td colspan="6">
<pre>
{$ihexrx}
</pre>
  </td>
</tr>
{/if}
    
