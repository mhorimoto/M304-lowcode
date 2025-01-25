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
    <h1>pkg_c編集</h1>
    <p>{$msg}</p>
    {include file='link_list.inc'}
    <table>
      <thead>
	<tr>
	  <th>PKG_ID</th><th>名前</th><th>コメント</th><th>日付</th><th class="num60">作者</th><th>操作</th>
	</tr>
      </thead>
      <tbody>
	{foreach from=$data item=row}
	<tr>
	  <form action="manup.php" method="POST">
	    <td><input type=NUMBER class="borderless pkg_id" name="PKG_ID" value="{$row.PKG_ID}"></td>
	    <td><input type=TEXT   class="borderless num200" name="NAME" value="{$row.NAME}"></td>
	    <td><input type=TEXT   class="borderless num300" name="COMMENT" value="{$row.COMMENT}"></td>
	    <td><input type=DATETIME-LOCAL class="borderless "name="REG_DATE" value="{$row.REG_DATE}"></td>
	    <td><input type=TEXT   class="borderless num60" name="AUTHOR" value="{$row.AUTHOR}"></td>
	    <td>
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="TBL" value="pkg_c">
	      <input type=SUBMIT name="EXEC" value="変更">
	      <input type=SUBMIT name="EXEC" value="削除">
	    </td>
	  </form>
	</tr>
	{/foreach}
	<form action="manup.php" method="POST">
	  <tr>
	    <td><input type=NUMBER class="borderless pkg_id" name="PKG_ID" placeholder="IDを入力"></td>
	    <td><input type=TEXT   class="borderless num200" name="NAME" placeholder="PKG名を入力"></td>
	    <td><input type=TEXT   class="borderless num300" name="COMMENT" placeholder="コメントを入力"></td>
	    <td><input type=DATETIME-LOCAL class="borderless "name="REG_DATE"></td>
	    <td><input type=TEXT   class="borderless num60" name="AUTHOR" placeholder="作成者名入力"></td>
	    <td>
	      <input type=HIDDEN name="SEQ" value="0">
	      <input type=SUBMIT name="EXEC" value="追加">
	      <input type=HIDDEN name="TBL" value="pkg_c">
	    </td>
	  </tr>
	</form>
      </tbody>
    </table>
  </body>
</html>
