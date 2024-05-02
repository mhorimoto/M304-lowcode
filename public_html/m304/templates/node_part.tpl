<!-- NODE PART -->
<!-- -*- mode: html-mode; -*- -->
<table class="t04">
  <tr>
    <th class="t-right">Node Name</th>
    <td><input type="TEXT" name="INFO_NODENAME" size="18" value="{$INFOMETHOD.nodename}"></td>
  </tr>
  <tr>
    <th class="t-right">Vender Name</th>
    <td>
      <select name="INFO_VEN" size="1">
	{foreach $VEN as $vend}
	{html_options values=$vend.code output=$vend.name selected=$INFOMETHOD.vencode}
	{/foreach}
      </select>
    </td>
  </tr>
  <tr>
    <th class="t-right">UECS-ID</th>
    <td><input type="TEXT" name="INFO_UECSID" size="12" maxlength="12" value="{$INFOMETHOD.uecsid}" pattern="^[0-9A-Fa-f]*$">
      <span class="s2-color">RECOMMEND</span></td>
  </tr>
  <tr>
    <th class="t-right">MAC Address</th>
    <td>
<!--      <input type="TEXT" name="INFO_MAC" size="17" maxlength="17" value="{$INFOMETHOD.mac}"> -->
      <input type="TEXT" name="INFO_MAC" size="17" maxlength="17" value="{$INFOMETHOD.mac}" pattern="^[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].$">
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
