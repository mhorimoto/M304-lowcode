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
		<h1>受信評価CCMTYPE編集 【PKGID={$return_PTR}】</h1>
		<p><a href="m304body2.php?ID={$return_PTR}"">もどる</a></p>
		<p>{$msg}</p>
		<table>
			<thead>
                <tr>
                    <th class="w73">PKG ID</th><th>比較ID</th><th>有効</th><th>ROOM/REGION/ORDER</th><th>寿命</th><th>CCMTYPE</th><th>名称</th><th>操作</th>
                </tr>
			</thead>
			<tbody>
			<!-- 一覧表示 -->
			{foreach from=$data item=row}
				<form action="m304blk_c.php?PKG_ID={$return_PTR}" method="POST">
					<tr>
                        {if $row.ID==0}
                        <td rowspan="10">
							{$row.PKG_ID}<br>
							<a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_C&PKG_ID={$row.PKG_ID}" target="_blank">
								<input type="button" name="BLK_C_BUTTON" value="IH変換">
							</a>
							<a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_C&PKG_ID={$row.PKG_ID}&OD=F" target="_blank">
								<input type="button" name="BLK_C_BUTTON" value="IH更新">
							</a>
						</td>
					{/if}
                        <td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$row.ID}"></td>
                        <td>
                            {if $row.VALID==1}
                            {$checked="checked"}
                            {else}
                            {$checked=""}
                            {/if}
                            <input type=CHECKBOX class="borderless" name="VALID" {$checked} value="{$row.VALID}">
                        </td>
                        <td class="center-align">
                            <input type=NUMBER class="borderless num3c r_12" name="ROOM"  min=0 max=127 step=1 value="{$row.ROOM}">/
                            <input type=NUMBER class="borderless num3c r_12" name="REGION" min=0 max=127 step=1 value="{$row.REGION}">/
                            <input type=NUMBER class="borderless num5c r_12" name="ORD" min=0 max=30000 value="{$row.ORD}">
                        </td>
                        <td>
                            <select name="LIFECOUNT">
                              {html_options options=$lifecount_sel selected="{$row.LIFECOUNT}"}
                            </select>
                        </td>
                        <td><input type=TEXT class="borderless" maxlength=22 name="CCMTYPE" value="{$row.CCMTYPE}"></td>
                        <td><input type=TEXT class="borderless num200" name="NAME" value="{$row.NAME}"></td>
                        <td>
                            <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
                            <input type=HIDDEN name="PKG_ID" value="{$row.PKG_ID}">
                            <input type=HIDDEN name="TBL" value="blk_c">
							{if $row.EMPTY==1}
							<input type=SUBMIT name="EXEC" style="height:24px;" value="新規追加">
							{else}
							<input type=SUBMIT name="EXEC" style="height:24px;background-color: palegreen;" value="変更">
							<input type=SUBMIT name="EXEC" style="height:24px;background-color: lightpink;" value="削除">
							{/if}
                        </td>
                    </tr>
                </form>
            {/foreach}
		<!-- 新規入力 -->
		 {if $row.EMPTY==0}
			{$nxtid = $row.ID+1}
			<form action="m304blk_c.php?PKG_ID={$return_PTR}" method="POST">
				<tr>
					{if $row.ID==0}
					<td rowspan="10"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td>
					{/if}
					<td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$nxtid}"></td>
					<td>
						{if $row.VALID==1}
						{$checked="checked"}
						{else}
						{$checked=""}
						{/if}
						<input type=CHECKBOX class="borderless" name="VALID" {$checked} value="{$row.VALID}">
					</td>
					<td class="center-align">
						<input type=NUMBER class="borderless num3c r_12" name="ROOM"  min=0 max=127 step=1 value="{$row.ROOM}">/
						<input type=NUMBER class="borderless num3c r_12" name="REGION" min=0 max=127 step=1 value="{$row.REGION}">/
						<input type=NUMBER class="borderless num5c r_12" name="ORD" min=0 max=30000 value="{$row.ORD}">
					</td>
					<td>
						<select name="LIFECOUNT">
							{html_options options=$lifecount_sel selected="{$row.LIFECOUNT}"}
						</select>
					</td>
					<td><input type=TEXT class="borderless" maxlength=22 name="CCMTYPE" value=""></td>
					<td><input type=TEXT class="borderless num200" name="NAME" value=""></td>
					<td>
						<input type=HIDDEN name="PKG_ID" value="{$return_PTR}">
						<input type=HIDDEN name="TBL" value="blk_c">
						<input type=SUBMIT name="EXEC" style="height:24px;" value="新規追加">
					</td>
				</tr>
			</form>
			{/if}
			</tbody>
		</table>
	</body>
</html>
