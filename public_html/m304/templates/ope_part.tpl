<!-- Version: 0.50 -->
  <tr>
    <th>No.</th>
    <th width="500">条件設定&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#x2611;有効 CCMTYPE 条件 値</th>
<!--    <th>Lev</th> -->
    <th>ROOM/REGION/ORDER/LIFETIME</th>
  </tr>
  {for $idx=0 to 9}
  <tr>
    <td class="t-center">C0{$idx}</td>
    <td class="t-center">
<!--      <input type="CHECKBOX" name="OPE_VALID[{$idx}]" {$OPE_CHECKED[$idx]}> -->
      <input type="CHECKBOX" name="OPE_VALID[{$idx}]" {$OPEMETHOD[$idx].checked}>
      <input type="TEXT" name="OPE_CCMTYPE[{$idx}]" value="{$OPE_CCMTYPE[{$idx}]}">
      <select name="OPE_CMP[{$idx}]">
	{foreach $CMPOPE as $cope}
	{html_options values=$cope.ope output=$cope.name selected=$OPEMETHOD[$idx].cmpsel}
	{/foreach}
      </select>
      <input type="NUMBER" name="OPE_FVAL[{$idx}]" class="txt5" size="6" maxlength="6" step="0.1" value="{$OPE_FVAL[$idx]}">
      <input type="checkbox" name="OPE_SUM[{$idx}]" value="on" {$OPE_SUM_CHECKED[$idx]} disabled>(積算値)
    </td>
    <td class="t-center">
      <input type="NUMBER" name="OPE_ROOM[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$OPE_ROOM[$idx]}">/
      <input type="NUMBER" name="OPE_REGION[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$OPE_REGION[$idx]}">/
      <input type="NUMBER" name="OPE_ORD[{$idx}]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="{$OPE_ORD[$idx]}">/
      <input type="NUMBER" name="OPE_LIFECNT[{$idx}]" class="txt3" min="0" max="30" size="2" maxlength="2" value="{$OPE_LIFECNT[$idx]}">
  </tr>
  {/for}
  <input type="HIDDEN" name="IHEXOPE" value="{$ihexope}">
  {if $ihexope!="NON" and $ihexope!=""}
<tr>
  <td colspan="3">
    変換結果：
<pre>
{$ihexope}
</pre>
  </td>
</tr>
{/if}
