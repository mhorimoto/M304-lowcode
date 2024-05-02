<!-- NODE PART -->
<!-- -*- mode: html-mode; -*- -->
<table class="t04">
  <tr>
    <th class="t-right">Node Name</th>
    <td><input type="TEXT" name="afn" size="18" value="{$AFN}"></td>
  </tr>
  <tr>
    <th class="t-right">Vender Name</th>
    <td>
      <select name="ven" size="1">
	{foreach $VEN as $vend}
	{html_options values=$vend.code output=$vend.name selected=$VENCODE}
	{/foreach}
      </select>
    </td>
  </tr>
  <tr>
    <th class="t-right">UECS-ID</th>
    <td><input type="TEXT" name="uecsid" size="12" maxlength="12" value="01100C00000B" pattern="^[0-9A-Fa-f]*$">
      <span class="s2-color">RECOMMEND</span></td>
  </tr>
  <tr>
    <th class="t-right">MAC Address</th>
    <td>
      <input type="TEXT" name="mac" size="17" maxlength="17" value="{$MAC}" pattern="^[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].$">
      <span class="s2-color">REQUIRED</span></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>
      <span style="align:right;">
<!--	<a href="memmap.html" target="_MEMMAP">
	  <input type="button" value="HELP MEMORY MAP">
	</a> -->
      </span>
    </td>
  </tr>
</table>
<!-- END OF NODE PART -->
