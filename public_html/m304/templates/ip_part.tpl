<!-- IP PART -->
<table class="t04">
  <tr>
    <td class="t-right">
      DHCP
    </td>
    <td>
      <input type="checkbox" name="DHCPF" id="DHCPFid" checked>
    </td>
  </tr>
  <tr>
    <td class="t-right">
      FIXED IP ADDRESS
    </td>
    <td>
      {literal}
      <input type="TEXT" name="FIXIP" size="15" maxlength="15" pattern="\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}" id="FIXIPid" disabled>
      {/literal}
    </td>
  </tr>
  <tr>
    <td class="t-right">
      BITMASK
    </td>
    <td>
      {literal}
      <input type="TEXT" name="BITMASK" size="15" maxlength="15" pattern="\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}" id="BITMASKid" disabled>
      {/literal}
    </td>
  </tr>
  <tr>
    <td class="t-right">
      GATEWAY
    </td>
    <td>
      {literal}
      <input type="TEXT" name="GATEWAY" size="15" maxlength="15" pattern="\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}" id="GATEWAYid" disabled>
      {/literal}
    </td>
  </tr>
  <tr>
    <td class="t-right">
      DNS
    </td>
    <td>
      {literal}
      <input type="TEXT" name="DNS" size="15" maxlength="15" pattern="\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}" id="DNSid" disabled>
      {/literal}
    </td>
  </tr>
</table>
<!-- END OF IP PART -->
