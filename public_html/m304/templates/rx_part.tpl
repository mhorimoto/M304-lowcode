<tr>
  <th rowspan="2">No.</th>
  <th width="400px">反映時間帯</th>
  <th>桁数</th>
  <th>単位</th>
  <th>Lev</th>
  <th>ROOM/REGION/ORDER/PRIORITY</th>
</tr>
<tr>
  <th colspan="3">CCM名称　　　　　　　　　　　　　複合条件</th>
  <th colspan="2">リレー状態設定</th>
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
  <td><input type="NUMBER" name="RX_CAST[{$idx}]" class="txt3" size="2" min="0" max="18" value="{$RXCAST[$idx]}"></td>
  <td><input type="TEXT" name="RX_UNIT[{$idx}]" size="8" placeholder="単位" value="{$RXUNIT[$idx]}"></td>
  <td>
    <select name="RX_Lev[{$idx}]">
      {foreach $LV as $rxlev}
      {html_options values=$rxlev.v output=$rxlev.name selected=$RXLEVSEL}
      {/foreach}
    </select>
  </td>
  <td>
    <input type="NUMBER" name="RX_ROOM[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$RXROOM[$idx]}">/
    <input type="NUMBER" name="RX_REGION[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$RXREGION[$idx]}">/
    <input type="NUMBER" name="RX_ORD[{$idx}]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="{$RXORDER[$idx]}">/
    <input type="NUMBER" name="RX_PRIORITY[{$idx}]" class="txt3" min="0" max="30" size="2" maxlength="2" value="{$RXPRIORITY[$idx]}">
  </td>
</tr>
<tr>
  <td colspan="3">
    <input type="TEXT" name="RX_CCMTYPE[{$idx}]" size="20" value="{$RX_CCMT[{$idx}]}">
    &nbsp;条件：
    <select name="cond0[{$idx}]">
      {foreach $CMPFUNC as $cf}
      {html_options values=$cf.f output=$cf.name selected=$CMPFUNCSEL0[$idx]}
      {/foreach}
    </select>
    {for $i=1 to 4}
    <select name="cmp{$i}[{$idx}]">
      {foreach $FUNCOPE as $fo}
      {html_options values=$fo.f output=$fo.name selected=$FUNCOPESEL{$i}[$idx]}
      {/foreach}
    </select>
    <select name="cond{$i}[{$idx}]">
      {foreach $CMPFUNC as $cf}
      {html_options values=$cf.f output=$cf.name selected=$CMPFUNCSEL{$i}[$idx]}
      {/foreach}
    </select>
    {/for}
  </td>
  <td colspan="2">
    {for $rn=1 to 8}
    <select name="RX_RLY{$rn}[{$idx}]">
      {foreach $RLY as $rly}
      {html_options values=$rly.v output=$rly.name selected=$RLYCODE{$rn}[$idx]}
      {/foreach}
    </select>
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
    
