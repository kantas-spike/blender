
*******************
Reference API Usage
*******************

.. comment
   Blender has many interlinking data types which have an auto-generated reference API which often has the information
   you need to write a script, but can be difficult to use.
   This document is designed to help you understand how to use the reference API.

Blenderはたくさんの相互にリンクしているデータ型を持っています。それらは自動生成されたAPIリファレンスがあり、
スクリプトを書くために必要な情報が載っていますが、利用するのは難しいかもしれません。
この文書は、リファレンスAPIの使い方を理解するためのものです。


Reference API Scope
===================

.. comment
   The reference API covers :mod:`bpy.types`, which stores types accessed via :mod:`bpy.context` -- *the user context*
   or :mod:`bpy.data` -- *blend-file data*.

リファレンスAPIは :mod:`bpy.types` をカバーしています。
:mod:`bpy.types` は、:mod:`bpy.context` 経由でアクセスされる型を格納しています。
それは、*ユーザーコンテキスト* や :mod:`bpy.data` 、 *blendファイルのデータ* などです。

.. comment
   Other modules such as :mod:`bmesh` and :mod:`aud` are not using Blender's data API
   so this document doesn't apply to those modules.

:mod:`bmesh` や :mod:`aud` などの他のモジュールは、BlenderのデータAPIを使用しません。
そのため、この文書はそれらのモジュールにはあてはまりません。


Data Access
===========

.. comment
   The most common case for using the reference API is to find out how to access data in the blend-file.
   Before going any further it's best to be aware of ID data-blocks in Blender since you will often find properties
   relative to them.

リファレンスAPIを使用する、最も一般的なケースは、blendファイル内のデータへのアクセス方法を調べることです。
話をすすめる前に、BlenderのIDデータブロックについて知っておくと良いでしょう。
それは、IDデータブロックに関連するプロパティを調べることが多いためです。

ID Data
-------

.. comment
   ID data-blocks are used in Blender as top-level data containers.
   From the user interface this isn't so obvious, but when developing you need to know about ID data-blocks.
   ID data types include Scene, Group, Object, Mesh, Workspace, World, Armature, Image and Texture.
   For a full list see the subclasses of :class:`bpy.types.ID`.

IDデータブロックはトップレベルのデータコンテナとしてBlenderで使われます。
ユーザーインターフェイスからではあまりわかりませんが、これは、開発時には、IDデータブロックについて知っておく必要があります。
IDデータタイプは Scene, Group, Object, Mesh, Workspace, World, Armature, Image そして Texture を含みます。
完全なリストは :class:`bpy.types.ID` のサブクラスを参照してください。

.. comment
   Here are some characteristics ID data-blocks share:

ここでは、IDデータブロックが共有する特徴を紹介します:

.. comment
   - IDs are blend-file data, so loading a new blend-file reloads an entire new set of data-blocks.
   - IDs can be accessed in Python from ``bpy.data.*``.
   - Each data-block has a unique ``.name`` attribute, displayed in the interface.
   - Animation data is stored in IDs ``.animation_data``.
   - IDs are the only data types that can be linked between blend-files.
   - IDs can be added/copied and removed via Python.
   - IDs have their own garbage-collection system which frees unused IDs when saving.
   - When a data-block has a reference to some external data, this is typically an ID data-block.

- IDはblendファイルのデータです。そして、新しいblendファイルがロードされると、完全な新しいデータブロックのセットがリロードされる
- IDはPythonで ``bpy.data.*`` からアクセスされる
- 各データブロックは固有の ``.name`` 属性を持ち、インターフェイスに表示される
- アニメーションデータは ``.animation_data`` IDに保存される
- IDは、複数のblendファイル間を関連付けれる、唯一のデータタイプである
- IDはPythonを使って追加、複製、削除できる
- IDは自分自身のガベージコレクションシステムを持っており、未使用のIDは保存時に開放される
- データブロックが外部データへの参照を持つ場合、これは通常のIDデータブロックである

Simple Data Access
------------------

.. comment
   In this simple case a Python script is used to adjust the object's location.
   Start by collecting the information where the data is located.

ここでの単純なケースでは、Pythonスクリプトはオブジェクトの位置を調整するために使われます。
データがどこにあるか情報収集を始めましょう。

.. comment
   First find this setting in the interface ``Properties editor -> Object -> Transform -> Location``.
   From the button context menu select *Online Python Reference*, this will link you to:
   :class:`bpy.types.Object.location`.
   Being an API reference, this link often gives little more information than the tooltip, though some of the pages
   include examples (normally at the top of the page).
   But you now know that you have to use ``.location`` and that it's an array of three floats.

まず、 インターフェイスの ``プロパティエディタ -> オブジェクト -> Transform -> ロケーション`` にある設定を見付けましょう。
ボタンのコンテキストメニューの *Online Python Reference* を選びましょう。
:class:`bpy.types.Object.location` のリンクを開きます。
APIリファレンスであるため、このリンクは、ツールチップよりも少しだけ多くの情報を与えてくれます。
いくつかのページはサンプルも含んでいます。(通常はページの一番上にあります。)
しかし、今は ``.location`` を使用しなければならないこと、そして3つのフロートの配列であることがわかりました。

.. comment
   So the next step is to find out where to access objects, go down to the bottom of the page to the references section,
   for objects there are many references, but one of the most common places to access objects is via the context.
   It's easy to be overwhelmed at this point since there ``Object`` get referenced in so many places:
   modifiers, functions, textures and constraints.
   But if you want to access any data the user has selected
   you typically only need to check the :mod:`bpy.context` references.

そして、次のステップは、オブジェクトにアクセスするための場所を見付けることです。
ページの一番下にあるリファレンスセクションに移動します。オブジェクトには多くのリファレンスがあります。
しかし、オブジェクトにアクセスできる最も一般的な場所の1つは、コンテキスト経由です。
``Object`` は、modifiers, functions, textures や constraintsなど、あまりに多くの場所で参照されるため、この時点で圧倒されがちです。
しかし、ユーザーが選択したデータにアクセスしたいのなら、通常、 :mod:`bpy.context` のリファレンスをチェックする必要があります。

.. comment
   Even then, in this case there are quite a few though
   if you read over these you'll notice that most are mode specific.
   If you happen to be writing a tool that only runs in Weight Paint Mode,
   then using ``weight_paint_object`` would be appropriate.
   However, to access an item the user last selected, look for the ``active`` members,
   Having access to a single active member the user selects is a convention in Blender:
   e.g. ``active_bone``, ``active_pose_bone``, ``active_node``, etc. and in this case you can use ``active_object``.

それでも、このケースでは、かなり多くの数がありますが、これらのリファレンスを読み返せば、ほとんどが、モード固有のもであると気づきます。
もし、Weight Paint Modeだけで動くツールを書こうとしているなら、 ``weight_paint_object`` を使うのが適切です。
しかしながら、ユーザーが最後に選択したアイテムにアクセスするためには、 ``active`` メンバーを探します。
ユーザーが選択した1つのアクティブメンバーにアクセスすることは、Blenderの慣例です:
例えば、 ``active_bone``, ``active_pose_bone``, ``active_node``などです。
そして、今回のケースでは、 ``active_object`` が使えます。

.. comment
   So now you have enough information to find the location of the active object.

これで、今、アクティブオブジェクトの位置を見付けるための十分な情報を得ました。

.. code-block:: python

   bpy.context.active_object.location

.. comment
   You can type this into the Python console to see the result.
   The other common place to access objects in the reference is :class:`bpy.types.BlendData.objects`.

Pythonコンソールに、これを入力して、結果を確認できます。
他の一般的なオブジェクトにアクセスできる場所は :class:`bpy.types.BlendData.objects` です。

.. comment
   .. note::

      This is **not** listed as :mod:`bpy.data.objects`,
      this is because :mod:`bpy.data` is an instance of the :class:`bpy.types.BlendData` class,
      so the documentation points there.

.. note::

   これは、:mod:`bpy.data.objects`  としてリストアップされていません。
   なぜなら :mod:`bpy.data` は  :class:`bpy.types.BlendData` クラスのインスタンスだからです。
   そのため、そのドキュメントはそちらにあります。

.. comment
   With :mod:`bpy.data.objects`, this is a collection of objects so you need to access one of its members:

:mod:`bpy.data.objects` では、オブジェクトのコレクションなので、そのメンバーの1つにアクセスする必要があります:

.. code-block:: python

   bpy.data.objects["Cube"].location


Nested Properties
-----------------

.. comennt
   The previous example is quite straightforward because ``location`` is a property of ``Object`` which can be accessed
   from the context directly.

前回の例では、 ``location`` が  コンテキストから直接アクセスできる ``Object``  のプロパティなので非常にわかりやすいです。

.. comment
   Here are some more complex examples:

こちらはもう少し複雑な例です:

.. code-block:: python

   # Access the number of samples for the Cycles render engine.
   bpy.context.scene.cycles.samples

   # Access to the current weight paint brush size.
   bpy.context.tool_settings.weight_paint.brush.size

   # Check if the window is full-screen.
   bpy.context.window.screen.show_fullscreen

.. comment
   As you can see there are times when you want to access data which is nested
   in a way that causes you to go through a few indirections.
   The properties are arranged to match how data is stored internally (in Blender's C code) which is often logical
   but not always quite what you would expect from using Blender.
   So this takes some time to learn, it helps you understand how data fits together in Blender
   which is important to know when writing scripts.

このように、ネストされたデータにアクセスする場合、いくつかの関節処理を経てアクセスしたい場合があります。
プロパティは、内部的(BlenderのCコード内で)にデータがどのように保存されるかにあわせて配置されます。
それは、論理的ですが、Blenderを使う時に期待されるものと必ずしも一致しません。
そのため、少し学習する時間がかかりますが、
Blender内でデータがどのように組み合わされるかを理解するのに役立ち、スクリプトを書くときに知っておくことが大切です。

.. comment
   When starting out scripting you will often run into the problem
   where you're not sure how to access the data you want.
   There are a few ways to do this:

スクリプティングを開始する時に、あなたはよく、
どのように望むデータにアクセスするべきか、よくわからないという問題にぶつかるでしょう。
これには、いくつか方法があります:

.. comment
  - Use the Python console's auto-complete to inspect properties.
    *This can be hit-and-miss but has the advantage
    that you can easily see the values of properties and assign them to interactively see the results.*
  - Copy the data path from the user interface.
    *Explained further in* :ref:`Copy Data Path <info_data_path_copy>`.
  - Using the documentation to follow references.
    *Explained further in* :ref:`Indirect Data Access <info_data_path_indirect>`.

- Pythonコンソールのオートコンプリートを使って、プロパティを調査する。
  *これは計画性はありませんが、簡単に、簡単にプロパティの値を確認したり、プロパティに値を設定しインタラクティブに結果を確認できるとうい、利点があります。*
- ユーザーインターフェイスからデータパスをコピーする。
  :ref:`Copy Data Path <info_data_path_copy>` で *さらに説明する*
- リファレンスを追うためにドキュメントを使用する
  :ref:`Indirect Data Access <info_data_path_indirect>` で *さらに説明する*

.. _info_data_path_copy:

Copy Data Path
--------------

.. comment
   Blender can compute the Python string to a property which is shown in the tooltip,
   on the line below ``Python: ...``. This saves having to open the API references to find where data is accessed from.
   In the context menu is a copy data-path tool which gives the path from an :class:`bpy.types.ID` data-block,
   to its property.

Blenderは、プロパティに対応するPython文字列を計算することができます。その文字列をツールチップの``Python: ...`` に続く行に表示します。
これは、どこからデータアクセスすべきか調べるためにAPIリファレンスを開く手間を省きます。
コンテキストメニュー内には、データパスをコピーするツールがあり、 :class:`bpy.types.ID` データブロックからのプロパティへのパス提供します。

.. comment
   To see how this works you'll get the path to the Subdivision Surface modifiers *Levels* setting.
   Start with the default scene and select the Modifiers tab, then add a Subdivision Surface modifier to the cube.
   Now hover your mouse over the button labeled *Levels Viewport*,
   The tooltip includes :class:`bpy.types.SubsurfModifier.levels` but you want the path from the object to this property.

この動作を確認するには、Subdivision Surface modifiers *Levels* の設定へのパスを取得します。
デフォルトシーンを開始し、Modifiersタブを選択します。そして、Subdivision Surface modifier をキューブに追加します。
そして、 *Levels Viewport* ボタンにマウスオーバーします。
ツールチップには、  :class:`bpy.types.SubsurfModifier.levels`  が含まれています。
しかし、あなたが欲しいのは、オブジェクトからこのプロパティまでのパスです。

.. comment
   Note that the text copied won't include the ``bpy.data.collection["name"].`` component since its assumed that
   you won't be doing collection look-ups on every access and typically you'll want to use the context rather
   than access each :class:`bpy.types.ID` instance by name.

コピーしたテキストは  ``bpy.data.collection["name"].`` 部分を含んでいません。
アクセスの度にコレクションを検索せずに、通常は、名前を使って :class:`bpy.types.ID` のインスタンスにアクセスするよりも、コンテキストを使うでしょう。

.. comment
   Type in the ID path into a Python console :mod:`bpy.context.active_object`.
   Include the trailing dot and don't execute the code, yet.

PythonコンソールにIDのパス  :mod:`bpy.context.active_object` を入力します。
末尾のドットは含めます。そして、まだ実行しないでください。

.. comment
   Now in the button's context menu select *Copy Data Path*, then paste the result into the console:

そして、ボタンのコンテキストメニューで  *Copy Data Path* を選択します。
それから結果をコンソールにペーストします。

.. code-block:: python

   bpy.context.active_object.modifiers["Subdivision"].levels

.. comment
   Press :kbd:`Return` and you'll get the current value of 1. Now try changing the value to 2:

:kbd:`Return` を押すと、現在値である1を取得できます。
それでは、値を2に変更しましょう。

.. code-block:: python

   bpy.context.active_object.modifiers["Subdivision"].levels = 2

.. comment
   You can see the value update in the Subdivision Surface modifier's UI as well as the cube.

Subdivision Surface modifier のUI内の値と同様にキューブも更新されたことがわかります。


.. _info_data_path_indirect:

Indirect Data Access
--------------------

.. comment
   This more advanced example shows the steps to access the active sculpt brushes texture.
   For example, if you want to access the texture of a brush via Python to adjust its ``contrast``.

このより高度な例では、active sculpt brushes texture にアクセスするためのステップを示します。
例えば、 ``contrast`` を調整するためにPython経由でブラシのテクスチャにアクセスしたい場合です。

.. comment
  #. Start in the default scene and enable Sculpt Mode from the 3D Viewport header.
  #. From the Sidebar expand the Brush Settings panel's *Texture* subpanel and add a new texture.
     *Notice the texture data-block menu itself doesn't have very useful links (you can check the tooltips).*
  #. The contrast setting isn't exposed in the Sidebar, so view the texture in the
     :ref:`Properties Editor <blender_manual:bpy.types.Texture.contrast>`.
  #. Open the context menu of the contrast field and select *Online Python Reference*.
     This takes you to ``bpy.types.Texture.contrast``. Now you can see that ``contrast`` is a property of texture.
  #. To find out how to access the texture from the brush check on the references at the bottom of the page.
     Sometimes there are many references, and it may take some guesswork to find the right one,
     but in this case it's ``tool_settings.sculpt.brush.texture``.
  #. Now you know that the texture can be accessed from ``bpy.data.brushes["BrushName"].texture``
     but normally you *won't* want to access the brush by name, instead you want to access the active brush.
     So the next step is to check on where brushes are accessed from via the references.

#. デフォルトシーンを開始し、3DビューポーヘッダでSculptモードを有効にする
#. サイドバーの Brush Settings panel's *Texture* サブパネルを開き、新しいテクスチャを追加する
   *注意: テクスチャデータブロックメニューは役に立つリンクを持っていません(ツールチップで確認できる) *
#. コントラストセッティングはサイドバーに表示されないため、:ref:`Properties Editor <blender_manual:bpy.types.Texture.contrast>` のテクスチャを表示する
#. コントラストフィールドのコンテキストメニューを開き、 *Online Python Reference* を選択する
   ``bpy.types.Texture.contrast``が表示され、 ``contrast`` がテキストのプロパティだとわかりました。
#. ブラシからテクスチャへのアクセス方法を調べるには、ページの一番下にあるリファレンスをチェックする
   たまに大量のリファレンスがある場合もあります。正しいアクセス方法を見つけるには、多少推測が必要かもれません
   しかし、このケースでは、``tool_settings.sculpt.brush.texture`` になります
#. 今、 ``bpy.data.brushes["BrushName"].texture`` からテクスチャにアクセス可能であるとわかりました。
   しかし、通常ブラシに名前でアクセスしたいと思わず、代りにアクティブブラシにアクセスしたい思うでしょう。
   そこで、次のステップはリファレンス経由でどこからブラシがアクセスされるかチェックします。

.. comment
   Now you can use the Python console to form the nested properties needed to access brush textures contrast:
   :menuselection:`Context --> Tool Settings --> Sculpt --> Brush --> Texture --> Contrast`.

今は、Pythonコンソールを使って、ブラシのテクスチャのコントラストへのアクセスに必要なネストされたプロパティを作ることができます:
:menuselection:`Context --> Tool Settings --> Sculpt --> Brush --> Texture --> Contrast`

.. comment
   Since the attribute for each is given along the way you can compose the data path in the Python console:

各属性は、途中で設定できるので、Pythonコンソール内でデータパスを作成できます:

.. code-block:: python

   bpy.context.tool_settings.sculpt.brush.texture.contrast

.. comment
   Or access the brush directly:

もしくは、直接ブラシにアクセスするなら:

.. code-block:: python

   bpy.data.textures["Texture"].contrast

.. comment
   If you are writing a user tool normally you want to use the :mod:`bpy.context` since the user normally expects
   the tool to operate on what they have selected.
   For automation you are more likely to use :mod:`bpy.data` since you want to be able to access specific data and
   manipulate it, no matter what the user currently has the view set at.

もしユーザーツールを作成するのなら、通常は :mod:`bpy.context` を使います。
利用者は通常、自分が選択しているものに対しツールが動作することを期待するからです。
自動化のためなら、ユーザーの現在のビュー設定に関係なく、特定のデータにアクセスして操作するようにしたいので :mod:`bpy.data` を使用することが多くなります。


Operators
=========

.. comment
   Most hotkeys and buttons in Blender call an operator which is also exposed to Python via :mod:`bpy.ops`.

Blenderのほとんどのホットキーやボタンは、 :mod:`bpy.ops` 経由でPythonに公開されている、オペレーターを呼び出します。

.. comment
   To see the Python equivalent hover your mouse over the button and see the tooltip,
   e.g ``Python: bpy.ops.render.render()``,
   If there is no tooltip or the ``Python:`` line is missing then this button is not using an operator
   and can't be accessed from Python.

Pythonに相当する機能を見るには、ボタンの上にマウスを置くと、ツールチップが表示されます。
例えば、 ``Python: bpy.ops.render.render()``
もし、ツールチップがなかったり、 ``Python:`` ではじまる行がなければ、
このボタンはオペレーターを使っておらず、Pythonからアクセスできません。

.. comment
   If you want to use this in a script you can press :kbd:`Ctrl-C` while your mouse is over the button
   to copy it to the clipboard.
   You can also use button's context menu and view the *Online Python Reference*, this mainly shows arguments and
   their defaults, however, operators written in Python show their file and line number which may be useful if you
   are interested to check on the source code.

もし、これをスクリプトで使いたければ、 マウスがボタン上にある間に、:kbd:`Ctrl-C` 押し、クリップボードにコピーします。
ボタンのコンテキストメニューを使って、 *Online Python Reference* を見ることもできます。
これは主に、引数とそのデフォルト値を確認します。
しかし、Pythonで書かれたオペレーターは、自身のソースと行番号を表示します。
これは、そのソースコードに興味がある場合は便利かもしれません。

.. comment
   .. note::

      Not all operators can be called usefully from Python,
      for more on this see :ref:`using operators <using_operators>`.

.. note::
   全てのオペレーターをPythonから呼べるわけではありません。
   詳細は  :ref:`using operators <using_operators>` を参照してください。

Info Editor
-----------

.. comment
   Blender records operators you run and displays them in the Info editor.
   Select the Scripting workspace that comes default with Blender to see its output.
   You can perform some actions and see them show up -- delete a vertex for example.

Blenderは実行されたオペレーターを記録し、Infoエディタに表示します。
Blenderのスクリプティングワークスペースを選択して、その出力を見ましょう。
いつくたアクション 例えば、頂点の削除 を実行すると、それらが表示されます。

.. comment
   Each entry can be selected, then copied :kbd:`Ctrl-C`, usually to paste in the text editor or Python console.

各エントリーは選択できます。そして、:kbd:`Ctrl-C` でコピーできます。普段は、テキストエディタやPythonコンソールに貼り付けます。

.. comment
   .. note::

      Not all operators get registered for display,
      zooming the view for example isn't so useful to repeat so it's excluded from the output.

      To display *every* operator that runs see :ref:`Show All Operators <info_show_all_operators>`.

.. note::
   全てのオペレーターが表示用に登録されるわけではありません、
   例えば、ビューのズームは、報告されても意味がないので、出力から除外されます。

   *全て* の実行されたオペレーターを表示するためには、 :ref:`Show All Operators <info_show_all_operators>` を参照してください
