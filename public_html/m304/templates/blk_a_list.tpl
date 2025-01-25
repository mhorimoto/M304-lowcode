<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>M304 parameter compiler</title>
    <meta name="description" content="https://at.strix-inc.jp/8487/">
    <meta name="keywords" content="M304">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="format-detection" content="telephone=no,email=no,address=no">

    <!-- Import Google Fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="
stylesheet">

    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">

    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <h1>blk_a編集</h1>
    <p>{$msg}</p>
    {include file='link_list.inc'}
    <table>
      <thead>
	<tr>
	  <th rowspan="3">機台番号</th><th>使用PKG</th><th>MAC</th><th>ベンダー名</th>
	  <th>ノード名</th><th>UECSID</th><th rowspan="3">操作</th>
	</tr>
	<tr>
	  <th>DHCP</th><th>IPアドレス</th><th>NETMASK</th><th>GATEWAY</th><th>DNS</th>
	</tr>
	<tr>
	  <th colspan="2">FARM ID</th><th colspan="3">コメント</th>
	</tr>
      </thead>
      <tbody>
	<!-- 一覧表示 -->
	{foreach from=$data item=row}
	<form action="manup.php" method="POST">
	<tr>
	    <td rowspan="3"><input type=NUMBER class="borderless num60 right-align" name="M304ID" value="{$row.M304ID}"></td>
	    <td>
	      {html_options name=PKG_ID options=$pindex selected=$row.PKG_ID}
	    </td>
	    <td><input type=TEXT class="borderless center-align" name="MACADDR" pattern="[0-9a-fA-F:]*" value="{$row.MACADDR}"></td>
	    <td><input type=TEXT class="borderless" name="VENDER_NAME" value="{$row.VENDER_NAME}"></td>
	    <td><input type=TEXT class="borderless" name="NODE_NAME" value="{$row.NODE_NAME}"></td>
	    <td><input type=TEXT class="borderless num100" name="UECSID" value="10100C00000B" readonly></td>
	    <td rowspan="3">
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="TBL" value="blk_a">
	      <input type=SUBMIT name="EXEC" value="変更"><br>
	      <input type=SUBMIT name="EXEC" value="削除">
	    </td>
	</tr>
	<tr>
	  <td>
	    {html_options name=DHCPFLAG options=$dhcptbl selected=$row.DHCPFLAG}
	  </td>
	  <td class="center-align"><input type=TEXT class="borderless center-align" name="FIXED_IPADDRESS" value="{$row.FIXED_IPADDRESS|default:'0.0.0.0'}"></td>
	  <td class="center-align"><input type=TEXT class="borderless center-align" name="FIXED_NETMASK" value="{$row.FIXED_NETMASK|default:'0.0.0.0'}"></td>
	  <td class="center-align"><input type=TEXT class="borderless center-align" name="FIXED_DEFGW" value="{$row.FIXED_DEFGW|default:'0.0.0.0'}"></td>
	  <td class="center-align"><input type=TEXT class="borderless center-align" name="FIXED_DNS" value="{$row.FIXED_DNS|default:'0.0.0.0'}"></td>
	</tr>
	<tr>
	  <td colspan="2">
	    {html_options name=FARM_ID options=$findex selected=$row.FARM_ID}
	  </td>
	  <td colspan="3" class="left-align">
	    <input type=TEXT class="borderless" name="COMMENT" value="{$row.COMMENT}">
	  </td>
	</tr>
	</form>
	{/foreach}
	<!-- 新規入力 -->
	<form action="manup.php" method="POST">
	  <tr>
	    <td rowspan="3"><input type=NUMBER class="borderless num60" name="M304ID"></td>
	    <td>
	      {html_options name=PKG_ID options=$pindex selected=0}
	    </td>
	    <td><input type=TEXT class="borderless" name="MACADDR" pattern="[0-9a-fA-F:]*"></td>
	    <td><input type=TEXT class="borderless" name="VENDER_NAME"></td>
	    <td><input type=TEXT class="borderless" name="NODE_NAME"></td>
	    <td><input type=TEXT class="borderless num100" name="UECSID" value="10100C00000B" readonly></td>
	    <td rowspan="3">
	      <input type=HIDDEN name="SEQ" value="0">
	      <input type=SUBMIT name="EXEC" value="追加">
	      <input type=HIDDEN name="TBL" value="blk_a">
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {html_options name=DHCPFLAG options=$dhcptbl selected=255}
	    </td>
	    <td><input type=TEXT class="borderless" name="FIXED_IPADDRESS" placeholder="xxx.xxx.xxx.xxx"></td>
	    <td><input type=TEXT class="borderless" name="FIXED_NETMASK" placeholder="xxx.xxx.xxx.xxx"></td>
	    <td><input type=TEXT class="borderless" name="FIXED_DEFGW" placeholder="xxx.xxx.xxx.xxx"></td>
	    <td><input type=TEXT class="borderless" name="FIXED_DNS" placeholder="xxx.xxx.xxx.xxx"></td>
	  </tr>
	  <tr>
	    <td colspan="2">
	      {html_options name=FARM_ID options=$findex selected=6}
	    </td>
	    <td colspan="3" class="left-align">
	      <input type=TEXT class="borderless" name="COMMENT" placeholder="コメントを入力">
	    </td>
	  </tr>
	</form>
      </tbody>
    </table>
  </body>
</html>

