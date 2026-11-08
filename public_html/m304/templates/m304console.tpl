<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>M304-{$data_a.M304ID} console</title>
    <meta name="description" content="https://at.strix-inc.jp/8487/">
    <meta name="keywords" content="M304">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="format-detection" content="telephone=no,email=no,address=no">
        
    <!-- Import Google Fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="stylesheet">
    
    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">
    
    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <table class="table-body">
      <tbody>
        {if $data_a.DHCPFLAG==0}
        {$dhcpf = "固定IP"}
        {else}
        {$dhcpf = "DHCP"}
        {/if}
        <input type="hidden" name="MACADDR" value="{$data_a.MACADDR}">
        <tr>
          <th rowspan="4">
	    機台番号<br>
	    <span style="font-size: 32px;">{$data_a.M304ID}</span><br>
	    <a href="m304body2.php?ID={$data_a.M304ID}" target="_self">
	      <input type="button" value="設定変更">
	    </a>
	  </th>
          <th colspan="2">{$data_a.NODE_NAME}</th><th colspan="3">{$data_custom.NAME}</th>
        </tr>
        <tr>
          <th>{$dhcpf}</th><th>IP:{$data_a.FIXED_IPADDRESS}</th>
          <th>NETMASK:{$data_a.FIXED_NETMASK}</th><th>GW:{$data_a.FIXED_DEFGW}</th><th>DNS:{$data_a.FIXED_DNS}</th>
        </tr>
        <tr>
          <td colspan="5">MACADDR:{$data_a.MACADDR}</td>
        </tr>
        <tr>
          <th colspan="5">{$data_a.COMMENT|default:'<br>'}</th>
        </tr>
      </tbody>
    </table>
    <table class="table-body">
      <tr>
        <td>動作状況</td>
        <td colspan="8">
            <span id="{$target_node_id}" data-uecs-type="m304"></span> - <span id="{$target_node_id}-ftime"> </span> 現在
        </td>
      </tr>
      <tr>
        <td rowspan="2">リレー状況</td>
	{for $i=1 to 8}
        <td>RLY{$i}<br>{$RLYname[$i]}</td>
	{/for}
      </tr>
      <tr>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="0"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="1"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="2"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="3"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="4"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="5"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="6"></span></td>
        <td><span data-uecs-id="{$target_node_id}" data-uecs-bit="7"></span></td>
      </tr>
    </table>
    <hr>
    <footer>&copy; 2026 YS Lab Version 0.51</footer>
    <script src="../js/uecs-ws.js"></script>
  </body>
</html>
