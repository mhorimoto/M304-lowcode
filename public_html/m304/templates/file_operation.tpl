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
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="stylesheet">
    
    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">
    
    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <h1 style="border:0;">M304 - {$data_a.M304ID}号機ファイル操作</h1>
    <p>{$msg}</p>
    <table>
      <tr><th>操作</th><th>ファイル名</th><th>コメント</th><th>オーナ</th><th>日時</th></tr>
      {foreach from=$save_dir item=row}
      <tr>
	<td>
	  <form method="POST" action="file_operation.php?ID={$data_a.M304ID}">
	    <input type="hidden" name="FILENAME" value="{$row.FILENAME}">
	    <input type="submit" name="OPEB" value="L">
	    <input type="button" name="OPEB" value="C">
	    <input type="submit" name="OPEB" value="D">
	  </form>
	</td>
	<td class="left-align">
	  {$row.FILENAME}
	</td>
	<td class="left-align">
	  {$row.COMMENT}
	</td>
	<td class="left-align">
	  {$row.OWNER}
	</td>
	<td>
	  {$row.TOD}
	</td>
      </tr>
      {/foreach}
    </table>
    <p><a href="m304body2.php?ID={$data_a.M304ID}">もどる</a></p>
  </body>
</html>
