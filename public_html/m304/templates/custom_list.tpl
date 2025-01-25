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
    <h1>tbl_custom編集</h1>
    <p>{$msg}</p>
    {include file='link_list.inc'}
    <table>
      <thead>
	<tr>
	  <th>COM_ID</th><th>FARM_ID</th><th>名前</th><th>住所</th><th>操作</th>
	</tr>
      </thead>
      <tbody>
	{foreach from=$data item=row}
	<tr>
	  <form action="manup.php" method="POST">
	    <td><input type=NUMBER class="com_id" name="COM_ID" value="{$row.COM_ID}"></td>
	    <td class="center-align"><input type=HIDDEN name="FARM_ID" value="{$row.FARM_ID}">{$row.FARM_ID}</td>
	    <td><input type=TEXT   name="NAME" value="{$row.NAME}"></td>
	    <td><input type=TEXT   name="ADDRESS" value="{$row.ADDRESS}"></td>
	    <td>
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="TBL" value="custom">
	      <input type=SUBMIT name="EXEC" value="変更">
	      <input type=SUBMIT name="EXEC" value="削除">
	    </td>
	  </form>
	</tr>
	{/foreach}
	<form action="manup.php" method="POST">
	  <tr>
	    <td><input type=NUMBER class="com_id" name="COM_ID"></td>
	    <td><input type=HIDDEN name="FARM_ID">自動決定</td>
	    <td><input type=TEXT   name="NAME"></td>
	    <td><input type=TEXT   name="ADDRESS"></td>
	    <td>
	      <input type=HIDDEN name="SEQ" value="0">
	      <input type=SUBMIT name="EXEC" value="追加">
	      <input type=HIDDEN name="TBL" value="custom">
	    </td>
	  </tr>
	</form>
      </tbody>
    </table>
  </body>
</html>
