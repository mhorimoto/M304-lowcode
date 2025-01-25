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
		<h1>リレー動作定義編集 【PKGID={$return_PTR}】</h1>
		<p><a href="m304body2.php?ID={$return_PTR}"">もどる</a></p>
		<p>{$msg}</p>
		<table>
			<thead>
				<tr>
					<th rowspan="2">OPE ID</th>
					<th rowspan="2">機台番号</th>
					<th rowspan="2">動作名称</th>
					<th colspan="8">リレー状態</th>
					<th rowspan="2">コメント</th>
					<th rowspan="2">操作</th>
				</tr>
				<tr>
				{for $rn=1 to 8}
					<th>RLY{$rn}</th>
				{/for}
				</tr>
			</thead>
			<tbody>
			<!-- 一覧表示 -->
			{foreach from=$data item=row}
				<form action="m304ope.php?PKG_ID={$return_PTR}" method="POST">
					<tr>
						<td><input type=NUMBER class="borderless num60 right-align" name="OPE_ID" value="{$row.OPE_ID}"></td>
						<td><input type=NUMBER class="borderless num60 right-align" name="M304ID" value="{$row.M304ID}"></td>
						<td><input type=TEXT class="borderless" name="NAME" value="{$row.NAME}"></td>
						{foreach from=$row['RLYSTATUS'] item=rlyval key=rlykey}
						<td>
							<select name="RLY[{$rlykey}]">
							{html_options options=$rlystatus selected=$rlyval}
							</select>
						</td>
						{/foreach}
						<td><input type=TEXT class="borderless num100" name="COMMENT" value="{$row.COMMENT}"></td>
						<td>
							<input type=HIDDEN name="ID"  value="{$row.OPE_ID}">
							<input type=HIDDEN name="TBL" value="blk_ope">
							<input type=HIDDEN name="PKG_ID" value="{$return_PTR}">
							<input type=HIDDEN name="OPEC_SEQ" value={$row.OPEC_SEQ}>
							<input type=SUBMIT name="EXEC" style="height:44px;background-color: palegreen;" value="変更">
							<input type=SUBMIT name="EXEC" style="height:44px;background-color: lightpink;" value="削除">
						</td>
					</tr>
				</form>
			{/foreach}
			<!-- 新規入力 -->
			{$nxtid = $row.OPE_ID+1}
			<form action="m304ope.php?PKG_ID={$return_PTR}" method="POST">
				<tr>
					<td><input type=NUMBER class="borderless num60 right-align" name="OPE_ID" value="{$nxtid}"></td>
					<td><input type=NUMBER class="borderless num60 right-align" name="M304ID" value="{$row.M304ID}" readonly></td>
					<td><input type=TEXT class="borderless" name="NAME" placeholder="リレー動作組合せ名"></td>
					{for $rn=1 to 8}
						{$rly="RLY[$rn]"}
					<td>
						{html_options name=$rly options=$rlystatus}
					</td>
					{/for}
					<td><input type=TEXT class="borderless" name="COMMENT" placeholder="短い説明"></td>
					<td>
						<input type=HIDDEN name="SEQ" value="0">
						<input type=SUBMIT name="EXEC" value="追加">
						<input type=HIDDEN name="PKG_ID" value="{$return_PTR}">
						<input type=HIDDEN name="TBL" value="blk_ope">
					</td>
				</tr>
			</form>
      </tbody>
    </table>
  </body>
</html>

