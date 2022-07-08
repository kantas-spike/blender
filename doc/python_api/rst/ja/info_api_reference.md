# Reference API Usage

Blenderはたくさんの相互にリンクしているデータ型を持っています。それらは自動生成されたAPIリファレンスがあり、
スクリプトを書くために必要な情報が載っていますが、利用するのは難しいかもしれません。
この文書は、リファレンスAPIの使い方を理解するためのものです。

## Reference API Scope

リファレンスAPIは `bpy.types` をカバーしています。 `bpy.types`
は、`bpy.context` 経由でアクセスされる型を格納しています。
それは、\*ユーザーコンテキスト\* や `bpy.data` 、
*blendファイルのデータ* などです。

`bmesh` や `aud`
などの他のモジュールは、BlenderのデータAPIを使用しません。
そのため、この文書はそれらのモジュールにはあてはまりません。

## Data Access

リファレンスAPIを使用する、最も一般的なケースは、blendファイル内のデータへのアクセス方法を調べることです。
話をすすめる前に、BlenderのIDデータブロックについて知っておくと良いでしょう。
それは、IDデータブロックに関連するプロパティを調べることが多いためです。

### ID Data

IDデータブロックはトップレベルのデータコンテナとしてBlenderで使われます。
ユーザーインターフェイスからではあまりわかりませんが、これは、開発時には、IDデータブロックについて知っておく必要があります。
IDデータタイプは Scene, Group, Object, Mesh, Workspace, World, Armature,
Image そして Texture を含みます。 完全なリストは `bpy.types.ID`
のサブクラスを参照してください。

ここでは、IDデータブロックが共有する特徴を紹介します:

-   IDはblendファイルのデータです。そして、新しいblendファイルがロードされると、完全な新しいデータブロックのセットがリロードされる
-   IDはPythonで `bpy.data.*` からアクセスされる
-   各データブロックは固有の `.name`
    属性を持ち、インターフェイスに表示される
-   アニメーションデータは `.animation_data` IDに保存される
-   IDは、複数のblendファイル間を関連付けれる、唯一のデータタイプである
-   IDはPythonを使って追加、複製、削除できる
-   IDは自分自身のガベージコレクションシステムを持っており、未使用のIDは保存時に開放される
-   データブロックが外部データへの参照を持つ場合、これは通常のIDデータブロックである

### Simple Data Access

ここでの単純なケースでは、Pythonスクリプトはオブジェクトの位置を調整するために使われます。
データがどこにあるか情報収集を始めましょう。

まず、 インターフェイスの
`プロパティエディタ -> オブジェクト -> Transform -> ロケーション`
にある設定を見付けましょう。 ボタンのコンテキストメニューの *Online
Python Reference* を選びましょう。 `bpy.types.Object.location`
のリンクを開きます。
APIリファレンスであるため、このリンクは、ツールチップよりも少しだけ多くの情報を与えてくれます。
いくつかのページはサンプルも含んでいます。(通常はページの一番上にあります。)
しかし、今は `.location`
を使用しなければならないこと、そして3つのフロートの配列であることがわかりました。

そして、次のステップは、オブジェクトにアクセスするための場所を見付けることです。
ページの一番下にあるリファレンスセクションに移動します。オブジェクトには多くのリファレンスがあります。
しかし、オブジェクトにアクセスできる最も一般的な場所の1つは、コンテキスト経由です。
`Object` は、modifiers, functions, textures や
constraintsなど、あまりに多くの場所で参照されるため、この時点で圧倒されがちです。
しかし、ユーザーが選択したデータにアクセスしたいのなら、通常、
`bpy.context` のリファレンスをチェックする必要があります。

それでも、このケースでは、かなり多くの数がありますが、これらのリファレンスを読み返せば、ほとんどが、モード固有のもであると気づきます。
もし、Weight Paint Modeだけで動くツールを書こうとしているなら、
`weight_paint_object` を使うのが適切です。
しかしながら、ユーザーが最後に選択したアイテムにアクセスするためには、
`active` メンバーを探します。
ユーザーが選択した1つのアクティブメンバーにアクセスすることは、Blenderの慣例です:
例えば、 `active_bone`, `active_pose_bone`, `active_node`などです。
そして、今回のケースでは、 `active_object` が使えます。

これで、今、アクティブオブジェクトの位置を見付けるための十分な情報を得ました。

``` python
bpy.context.active_object.location
```

Pythonコンソールに、これを入力して、結果を確認できます。
他の一般的なオブジェクトにアクセスできる場所は
`bpy.types.BlendData.objects` です。

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

これは、`bpy.data.objects` としてリストアップされていません。 なぜなら
`bpy.data` は `bpy.types.BlendData` クラスのインスタンスだからです。
そのため、そのドキュメントはそちらにあります。

</div>

`bpy.data.objects`
では、オブジェクトのコレクションなので、そのメンバーの1つにアクセスする必要があります:

``` python
bpy.data.objects["Cube"].location
```

### Nested Properties

前回の例では、 `location` が コンテキストから直接アクセスできる `Object`
のプロパティなので非常にわかりやすいです。

こちらはもう少し複雑な例です:

``` python
# Access the number of samples for the Cycles render engine.
bpy.context.scene.cycles.samples

# Access to the current weight paint brush size.
bpy.context.tool_settings.weight_paint.brush.size

# Check if the window is full-screen.
bpy.context.window.screen.show_fullscreen
```

このように、ネストされたデータにアクセスする場合、いくつかの関節処理を経てアクセスしたい場合があります。
プロパティは、内部的(BlenderのCコード内で)にデータがどのように保存されるかにあわせて配置されます。
それは、論理的ですが、Blenderを使う時に期待されるものと必ずしも一致しません。
そのため、少し学習する時間がかかりますが、
Blender内でデータがどのように組み合わされるかを理解するのに役立ち、スクリプトを書くときに知っておくことが大切です。

スクリプティングを開始する時に、あなたはよく、
どのように望むデータにアクセスするべきか、よくわからないという問題にぶつかるでしょう。
これには、いくつか方法があります:

-   Pythonコンソールのオートコンプリートを使って、プロパティを調査する。
    *これは計画性はありませんが、簡単に、簡単にプロパティの値を確認したり、プロパティに値を設定しインタラクティブに結果を確認できるとうい、利点があります。*
-   ユーザーインターフェイスからデータパスをコピーする。
    `Copy Data Path <info_data_path_copy>` で *さらに説明する*
-   リファレンスを追うためにドキュメントを使用する
    `Indirect Data Access <info_data_path_indirect>` で *さらに説明する*

### Copy Data Path [info_data_path_copy]

Blenderは、プロパティに対応するPython文字列を計算することができます。その文字列をツールチップの`Python: ...`
に続く行に表示します。
これは、どこからデータアクセスすべきか調べるためにAPIリファレンスを開く手間を省きます。
コンテキストメニュー内には、データパスをコピーするツールがあり、
`bpy.types.ID` データブロックからのプロパティへのパス提供します。

この動作を確認するには、Subdivision Surface modifiers *Levels*
の設定へのパスを取得します。
デフォルトシーンを開始し、Modifiersタブを選択します。そして、Subdivision
Surface modifier をキューブに追加します。 そして、 *Levels Viewport*
ボタンにマウスオーバーします。 ツールチップには、
`bpy.types.SubsurfModifier.levels` が含まれています。
しかし、あなたが欲しいのは、オブジェクトからこのプロパティまでのパスです。

コピーしたテキストは `bpy.data.collection["name"].`
部分を含んでいません。
アクセスの度にコレクションを検索せずに、通常は、名前を使って
`bpy.types.ID`
のインスタンスにアクセスするよりも、コンテキストを使うでしょう。

PythonコンソールにIDのパス `bpy.context.active_object` を入力します。
末尾のドットは含めます。そして、まだ実行しないでください。

そして、ボタンのコンテキストメニューで *Copy Data Path* を選択します。
それから結果をコンソールにペーストします。

``` python
bpy.context.active_object.modifiers["Subdivision"].levels
```

`Return` を押すと、現在値である1を取得できます。
それでは、値を2に変更しましょう。

``` python
bpy.context.active_object.modifiers["Subdivision"].levels = 2
```

Subdivision Surface modifier
のUI内の値と同様にキューブも更新されたことがわかります。

### Indirect Data Access [info_data_path_indirect]

このより高度な例では、active sculpt brushes texture
にアクセスするためのステップを示します。 例えば、 `contrast`
を調整するためにPython経由でブラシのテクスチャにアクセスしたい場合です。

1.  デフォルトシーンを開始し、3DビューポーヘッダでSculptモードを有効にする
2.  サイドバーの Brush Settings panel's *Texture*
    サブパネルを開き、新しいテクスチャを追加する *注意:
    テクスチャデータブロックメニューは役に立つリンクを持っていません(ツールチップで確認できる)*
3.  コントラストセッティングはサイドバーに表示されないため、`Properties Editor <blender_manual:bpy.types.Texture.contrast>`
    のテクスチャを表示する
4.  コントラストフィールドのコンテキストメニューを開き、 *Online Python
    Reference* を選択する `bpy.types.Texture.contrast`が表示され、
    `contrast` がテキストのプロパティだとわかりました。
5.  ブラシからテクスチャへのアクセス方法を調べるには、ページの一番下にあるリファレンスをチェックする
    たまに大量のリファレンスがある場合もあります。正しいアクセス方法を見つけるには、多少推測が必要かもれません
    しかし、このケースでは、`tool_settings.sculpt.brush.texture`
    になります
6.  今、 `bpy.data.brushes["BrushName"].texture`
    からテクスチャにアクセス可能であるとわかりました。
    しかし、通常ブラシに名前でアクセスしたいと思わず、代りにアクティブブラシにアクセスしたい思うでしょう。
    そこで、次のステップはリファレンス経由でどこからブラシがアクセスされるかチェックします。

今は、Pythonコンソールを使って、ブラシのテクスチャのコントラストへのアクセスに必要なネストされたプロパティを作ることができます:
`Context --> Tool Settings --> Sculpt --> Brush --> Texture --> Contrast`

各属性は、途中で設定できるので、Pythonコンソール内でデータパスを作成できます:

``` python
bpy.context.tool_settings.sculpt.brush.texture.contrast
```

もしくは、直接ブラシにアクセスするなら:

``` python
bpy.data.textures["Texture"].contrast
```

もしユーザーツールを作成するのなら、通常は `bpy.context` を使います。
利用者は通常、自分が選択しているものに対しツールが動作することを期待するからです。
自動化のためなら、ユーザーの現在のビュー設定に関係なく、特定のデータにアクセスして操作するようにしたいので
`bpy.data` を使用することが多くなります。

## Operators

Blenderのほとんどのホットキーやボタンは、 `bpy.ops`
経由でPythonに公開されている、オペレーターを呼び出します。

Pythonに相当する機能を見るには、ボタンの上にマウスを置くと、ツールチップが表示されます。
例えば、 `Python: bpy.ops.render.render()`
もし、ツールチップがなかったり、 `Python:` ではじまる行がなければ、
このボタンはオペレーターを使っておらず、Pythonからアクセスできません。

もし、これをスクリプトで使いたければ、
マウスがボタン上にある間に、`Ctrl-C`
押し、クリップボードにコピーします。
ボタンのコンテキストメニューを使って、 *Online Python Reference*
を見ることもできます。 これは主に、引数とそのデフォルト値を確認します。
しかし、Pythonで書かれたオペレーターは、自身のソースと行番号を表示します。
これは、そのソースコードに興味がある場合は便利かもしれません。

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

全てのオペレーターをPythonから呼べるわけではありません。 詳細は
`using operators <using_operators>` を参照してください。

</div>

### Info Editor

Blenderは実行されたオペレーターを記録し、Infoエディタに表示します。
Blenderのスクリプティングワークスペースを選択して、その出力を見ましょう。
いつくたアクション 例えば、頂点の削除
を実行すると、それらが表示されます。

各エントリーは選択できます。そして、`Ctrl-C`
でコピーできます。普段は、テキストエディタやPythonコンソールに貼り付けます。

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

全てのオペレーターが表示用に登録されるわけではありません、
例えば、ビューのズームは、報告されても意味がないので、出力から除外されます。

*全て* の実行されたオペレーターを表示するためには、
`Show All Operators <info_show_all_operators>` を参照してください

</div>
