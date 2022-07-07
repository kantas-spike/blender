.. _info_quickstart:

**********
Quickstart
**********

.. comment
   This :abbr:`API (Application Programming Interface)` is generally stable
   but some areas are still being extended and improved.

この :abbr:`API (Application Programming Interface)` は、おおむね安定していますが、一部の分野はまだ拡張・改良中です。

.. comment
   .. rubric:: Blender Python API features:

.. rubric:: Blender Python API の特徴:

.. comment
   - Edit any data the user interface can (Scenes, Meshes, Particles etc.).
   - Modify user preferences, keymaps and themes.
   - Run tools with own settings.
   - Create user interface elements such as menus, headers and panels.
   - Create new tools.
   - Create interactive tools.
   - Create new rendering engines that integrate with Blender.
   - Subscribe to changes to data and it's properties.
   - Define new settings in existing Blender data.
   - Draw in the 3D Viewport using Python.

- ユーザーインターフェイスで利用できる全てのデータを編集できる。 (Scenes, Meshes, Particles etc.)
- ユーザープリファレンスやキーマップ、テーマを変更できる。
- ツールを独自の設定で起動できる。
- メニューやヘッダ、パネルのようなユーザーインターフェイスの要素を作成できる。
- 新しいツールを作成できる。
- インタラクティブツールを作成できる。
- Blenderと統合可能な新しいレンダリングエンジンを作成できる。
- データやプロパティの変更を購読できる。
- 既存のBlenderデータに新しい設定を定義できる。
- Pythonを使って、3Dビューポートに描画できる。

.. comment
   .. rubric:: (Still) missing features:

.. rubric:: まだ、存在しない特徴:

.. comment
  - Create new space types.
  - Assign custom properties to every type.

- 新しいスペースタイプを作成できる。
- 全てのタイプにカスタムプロパティを割り当てることができる。

.. comment
   Before Starting
   ===============

   はじめる前に
   ===============

.. comment
   This document its intended to familiarize you with Blender Python API
   but not to fully cover each topic.

この文書は、Blender Python API に習熟できることを目的としていますが、
全てのトピックをカバーしているわけではありません。

.. comment
   A quick list of helpful things to know before starting:

はじめる前に知っておくと便利なことを簡単にまとめました:

.. comment
  - Enable :ref:`Developer Extra <blender_manual:prefs-interface-dev-extras>`
    and :ref:`Python Tooltips <blender_manual:prefs-interface-tooltips-python>`.
  - The :ref:`Python Console <blender_manual:bpy.types.SpaceConsole>`
    is great for testing one-liners; it has autocompletion so you can inspect the API quickly.
  - Button tooltips show Python attributes and operator names (when enabled see above).
  - The context menu of buttons directly links to this API documentation (when enabled see above).
  - Many python examples can be found in the text editor's template menu.
  - To examine further scripts distributed with Blender, see:

    - ``scripts/startup/bl_ui`` for the user interface.
    - ``scripts/startup/bl_operators`` for operators.

    Exact location depends on platform, see:
    :ref:`directory layout docs <blender_manual:blender-directory-layout>`.

- :ref:`Developer Extra <blender_manual:prefs-interface-dev-extras>`
  と :ref:`Python Tooltips <blender_manual:prefs-interface-tooltips-python>` の設定を有効化
- :ref:`Python Console <blender_manual:bpy.types.SpaceConsole>` はワンライナーをテストするのに大変便利です。
  自動補完機能があるのでAPIを素早く調べることができます。
- 上述の設定を有効にしていれば、ツールチップボタンがPythonの属性とオペレーターの名前を表示します。
- 上述の設定を有効にしていれば、ボタンのコンテキストメニューがAPIドキュメントに直接リンクされます。
- たくさんのPythonのサンプルがテキストエディタのテンプレートメニューでみつけることができます。
- Blenderとともに配布されている更なるスクリプトを調べるには以下を参照してください:

  - ``scripts/startup/bl_ui`` ユーザーインターフェイス用
  - ``scripts/startup/bl_operators`` オペレーター用.

  正確な位置はプラットフォームに依存しますので、以下を参照してください:
  :ref:`directory layout docs <blender_manual:blender-directory-layout>`.

.. comment
   Running Scripts
   ---------------

スクリプトを実行する
---------------------

.. comment
   The two most common ways to execute Python scripts are using the built-in
   text editor or entering commands in the Python console.
   Both the *Text Editor* and *Python Console* are space types you can select from the header.
   Rather than manually configuring your spaces for Python development,
   you can use the *Scripting* workspace accessible from the Topbar tabs.

Pythonスクリプトを実行する、最も一般的な2つ方法が、組み込みのテキストエディタを使う方法、とPythonコンソールにコマンド入力する方法です。
*テキストエディタ* と *Pythonコンソール* は共に、ヘッダから選択できるスペースのタイプにです。
手動で設定したPython開発用のスペースではなく、トップバータブからアクセスできる *スクリプティング* ワークスペースを使いましょう。

テキストエディタで、 ``.py`` ファイルを開くか、クリップボードから貼り付けて、 *Run Script* を使って試すことができます。
Python コンソールは、一般的に、スニペットの入力や、すぐにフィードバックを得るために使用しますが、
スクリプト全体を貼り付けることも可能です。
スクリプトはコマンドラインからBlenderとともに実行することも可能ですが、Blenderでのスクリプティングの学習において必須ではありません。

.. comment
   Key Concepts
   ============

キーコンセプト
==============

.. comment
   Data Access
   -----------

データアクセス
---------------

.. comment
   Accessing Data-Blocks
   ^^^^^^^^^^^^^^^^^^^^^

データブロックへのアクセス
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. comment
   You can access Blender's data with the Python API in the same way as the animation system or user interface;
   this implies that any setting that can be changed via a button can also be changed with Python.
   Accessing data from the currently loaded blend-file is done with the module :mod:`bpy.data`.
   It gives access to library data, for example:

Python API を使って、アニメーションシステムやユーザーインターフェイスと同じやり方でBlenderのデータにアクセスできます。
これは、ボタンで変更できるようなどのような設定も、Pythonで変更できることを意味します。
現在読み込まれているblendファイルのデータへのアクセスは、 :mod:`bpy.data` モジュールで行います。
このモジュールは、ライブラリデータへのアクセスを提供します。例えば:

   >>> bpy.data.objects
   <bpy_collection[3], BlendDataObjects>

   >>> bpy.data.scenes
   <bpy_collection[1], BlendDataScenes>

   >>> bpy.data.materials
   <bpy_collection[1], BlendDataMaterials>

.. comment
   Accessing Collections
   ^^^^^^^^^^^^^^^^^^^^^

コレクションへのアクセス
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. comment
   You will notice that an index as well as a string can be used to access members of the collection.
   Unlike Python dictionaries, both methods are available;
   however, the index of a member may change while running Blender.

インデックスと同じく文字列でコレクションのメンバーへのアクセスに使用できることにお気付きでしょう。
Pythonのディクショナリーと違い、どちらの方法も利用できます;
しかしながら、メンバーのインデックスは、Blenderの実行中に変わることがあります。

   >>> list(bpy.data.objects)
   [bpy.data.objects["Cube"], bpy.data.objects["Plane"]]

   >>> bpy.data.objects['Cube']
   bpy.data.objects["Cube"]

   >>> bpy.data.objects[0]
   bpy.data.objects["Cube"]


.. comment
   Accessing Attributes
   ^^^^^^^^^^^^^^^^^^^^

属性へのアクセス
^^^^^^^^^^^^^^^^^^^^

.. comment
   Once you have a data-block, such as a material, object, collection, etc.,
   its attributes can be accessed much like you would change a setting using the graphical interface.
   In fact, the tooltip for each button also displays the Python attribute
   which can help in finding what settings to change in a script.

マテリアルやオブジェクト、コレクションといったデータブロックを一度取得したら、
まるでGUIで設定を変更するかのように、その属性にアクセスできます。
実際、各ボタンのツールチップは、Pythonの属性を表示します。これはスクリプトでどこの設定を変更するべきか調べるさいに便利です。

   >>> bpy.data.objects[0].name
   'Camera'

   >>> bpy.data.scenes["Scene"]
   bpy.data.scenes['Scene']

   >>> bpy.data.materials.new("MyMaterial")
   bpy.data.materials['MyMaterial']

.. comment
   For testing what data to access it's useful to use the Python Console, which is its own space type.
   This supports auto-complete, giving you a fast way to explore the data in your file.

どのデータにアクセスするべきか試す場合は、Python コンソールの利用が便利です。Python コンソールは独自のスペースタイプです。
これは、オートコンプリートをサポートし、あなたのコード内のデータを素早く調査する手段を提供します。

.. comment
   Example of a data path that can be quickly found via the console:

コンソールですぐに確認できるデータパスの例:

   >>> bpy.data.scenes[0].render.resolution_percentage
   100
   >>> bpy.data.scenes[0].objects["Torus"].data.vertices[0].co.x
   1.0

.. comment
   Data Creation/Removal
   ^^^^^^^^^^^^^^^^^^^^^

データ作成/削除
^^^^^^^^^^^^^^^^^^^^^

.. comment
   When you are familiar with other Python APIs you may be surprised that
   new data-blocks in the bpy API cannot be created by calling the class:

他のPython APIに慣れている場合、
bpy APIで、該当クラスの呼び出しにより新しいデータブロックを作成できないことに驚くかもしれません:

   >>> bpy.types.Mesh()
   Traceback (most recent call last):
     File "<blender_console>", line 1, in <module>
   TypeError: bpy_struct.__new__(type): expected a single argument

.. comment
   This is an intentional part of the API design.
   The Blender Python API can't create Blender data that exists outside the main Blender database
   (accessed through :mod:`bpy.data`), because this data is managed by Blender (save, load, undo, append, etc).

これはAPI設計の意図的な部分です。
Blender Python API は メインのBlenderデータベース(:mod:`bpy.data` を通してアクセスされる)の外で、Blenderデータを作成できません。
このデータはBlenderによって管理されます。(保存、読み込み、アンドゥ、追加など)

.. comment
   Data is added and removed via methods on the collections in :mod:`bpy.data`, e.g:

:mod:`bpy.data` 内のコレクションのメソッドを経由して、データは追加、削除されます。例えば:

   >>> mesh = bpy.data.meshes.new(name="MyMesh")
   >>> print(mesh)
   <bpy_struct, Mesh("MyMesh.001")>

   >>> bpy.data.meshes.remove(mesh)

.. comment
   Custom Properties
   ^^^^^^^^^^^^^^^^^

カスタムプロパティ
^^^^^^^^^^^^^^^^^^^

.. comment
   Python can access properties on any data-block that has an ID
   (data that can be linked in and accessed from :mod:`bpy.data`).
   When assigning a property, you can pick your own names,
   these will be created when needed or overwritten if they already exist.

IDを持ついずれのデータブロック(:mod:`bpy.data`にリンクして、アクセスできるデータ)のプロパティに、Pythonはアクセスできます。
プロパティを割り当てる際に、独自の名前を選択できます。
これらは必要な時に作成され、すでに存在する場合は上書きされます。

.. comment
   This data is saved with the blend-file and copied with objects, for example:

このデータはblendファイルに保存され、オブジェクトとしてコピーされます。例えば:

.. code-block:: python

   bpy.context.object["MyOwnProperty"] = 42

   if "SomeProp" in bpy.context.object:
       print("Property found")

   # Use the get function like a Python dictionary
   # which can have a fallback value.
   value = bpy.data.scenes["Scene"].get("test_prop", "fallback value")

   # dictionaries can be assigned as long as they only use basic types.
   collection = bpy.data.collections.new("MyTestCollection")
   collection["MySettings"] = {"foo": 10, "bar": "spam", "baz": {}}

   del collection["MySettings"]

.. comment
   Note that these properties can only be assigned basic Python types:

これらのプロパティには、Pythonの基本型しか割り当てられないことに注意してください。

- int, float, string
- array of ints or floats
- dictionary (only string keys are supported, values must be basic types too)

.. comment
   These properties are valid outside of Python. They can be animated by curves or used in driver paths.

これらのプロパティはPythonの外でも有効です。
これらは曲線アニメーションやドライバーパスで使用することができます。


Context
-------

.. comment
   While it's useful to be able to access data directly by name or as a list,
   it's more common to operate on the user's selection.
   The context is always available from ``bpy.context`` and can be used to get the active object, scene,
   tool settings along with many other attributes.

名前やリストで直接データにアクセスできることは便利ですが、ユーザーが選択しているデータを操作することは、より一般的です。
コンテキストは ``bpy.context`` で、いつでも利用でき、
アクティブオブジェクト、シーンや他の多くの属性を持つツール設定の取得に使用されます。

.. comment
   Some common use cases are:

いくつかの一般的なユースケースは以下になります:

   >>> bpy.context.object
   >>> bpy.context.selected_objects
   >>> bpy.context.visible_bones

.. comment
   Note that the context is read-only, which means that these values cannot be modified directly.
   But they can be changed by running API functions or by using the data API.

コンテキストは読取専用です。これは、直接その値を変更できないことを意味します。
ですが、API関数の実行やデータAPIの使用により、値を変更可能です。

.. comment
   So ``bpy.context.active_object = obj`` will raise an error.
   But ``bpy.context.view_layer.objects.active = obj`` works as expected.

そのため、``bpy.context.active_object = obj`` はエラーになりますが、
``bpy.context.view_layer.objects.active = obj`` は期待通りに動きます。

.. comment
   The context attributes change depending on where they are accessed.
   The 3D Viewport has different context members than the Python Console,
   so take care when accessing context attributes that the user state is known.

コンテキストの属性は、アクセスしている場所によって変わります。
3Dビューポートは、Pythonコンソールと比べて、コンテキストのメンバーが異なります。
そのため、コンテキストの属性にアクセスするときは、ユーザーの状態を知るように注意してください。

.. comment
   See :mod:`bpy.context` API reference.

参照 :mod:`bpy.context` API リファレンス

Operators (Tools)
-----------------

.. comment
   Operators are tools generally accessed by the user from buttons, menu items or key shortcuts.
   From the user perspective they are a tool but Python can run these with its own settings
   through the :mod:`bpy.ops` module.

オペレーターは一般的に、ボタンやメニュー、キーショートカットによりユーザーがアクセスするツールのことです。
ユーザーの観点ではツールですが、Pythonは :mod:`bpy.ops` を通して、これらを独自の設定で実行することができます。

.. comment
   Examples:

例:

   >>> bpy.ops.mesh.flip_normals()
   {'FINISHED'}
   >>> bpy.ops.mesh.hide(unselected=False)
   {'FINISHED'}
   >>> bpy.ops.object.transform_apply()
   {'FINISHED'}

.. tip::

.. comment
   The :ref:`Operator Cheat Sheet <blender_manual:bpy.ops.wm.operator_cheat_sheet>`
   gives a list of all operators and their default values in Python syntax, along with the generated docs.
   This is a good way to get an overview of all Blender's operators.

:ref:`Operator Cheat Sheet <blender_manual:bpy.ops.wm.operator_cheat_sheet>` に全てのオペレーターとPython上のデフォルト値のリストと、生成されたドキュメントがあります。
これは、全てのBlenderオペレーターの概要を知る良い方法です。

Operator Poll()
^^^^^^^^^^^^^^^

.. comment
   Many operators have a "poll" function which checks if the cursor
   is in a valid area or if the object is in the correct mode (Edit Mode, Weight Paint Mode, etc).
   When an operator's poll function fails within Python, an exception is raised.

多くのオペレーターは"poll"関数を持ちます。
これは、カーソルが正しい場所にあるかどうか、オブジェクトが正しいモード(Editモード、ウェイトペイントモードなど)かどうかをチェックします。
Pythonでオペレーターのpoll関数が失敗した場合、例外が発生します。

.. comment
   For example, calling ``bpy.ops.view3d.render_border()`` from the console raises the following error:

例えば、 コンソールから``bpy.ops.view3d.render_border()`` を呼ぶと、以下の例外が発生します。

.. code-block:: python

   RuntimeError: Operator bpy.ops.view3d.render_border.poll() failed, context is incorrect

.. comment
   In this case the context must be the 3D Viewport with an active camera.

このケースでは、コンテキストはアクティブカメラを持つ3Dビューポートである必要があります。

.. comment
   To avoid using try-except clauses wherever operators are called, you can call the operators
   own ``poll()`` function to check if it can run the operator in the current context.

try-except節の使用を避けるためには、オペレーターを呼ぶ時はいつでも、
現在のコンテキストでオペレーターが実行できるかどうかチェックするためにオペレーター自身の ``poll()`` 関数を呼びます。

.. code-block:: python

   if bpy.ops.view3d.render_border.poll():
       bpy.ops.view3d.render_border()

.. comment
   Integration
   ===========

インテグレーション
===================

.. comment
   Python scripts can integrate with Blender in the following ways:

Pythonスクリプトは、以下の方法でBlenderと統合できます。

.. comment
  - By defining a render engine.
  - By defining operators.
  - By defining menus, headers and panels.
  - By inserting new buttons into existing menus, headers and panels.

- レンダリングエンジンを定義
- オペレーターを定義
- メニュー、ヘッダやパネルを定義
- 新しいボタンを既存のメニュー、ヘッダやパネルに追加

.. comment
   In Python, this is done by defining a class, which is a subclass of an existing type.

Pythonでは、統合は既存型のサブクラスを定義することで実行します。

.. comment
   Example Operator
   ----------------

オペレーターの例
-----------------

.. comment
   .. literalinclude:: __/__/__/__/release/scripts/templates_py/operator_simple.py

.. include:: ../../../../release/scripts/templates_py/operator_simple.py
   :code: python

.. comment
   Once this script runs, ``SimpleOperator`` is registered with Blender
   and can be called from Operator Search or added to the toolbar.

一度、このスクリプトを実行すると、 ``SimpleOperator`` がBlenderに登録されます。
そして、オペレーターの検索やツールバーに追加することで、利用できるようになります。

.. comment
   To run the script:

スクリプトを実行するには:

.. comment
  #. Start Blender and switch to the Scripting workspace.
  #. Click the *New* button in the text editor to create a new text data-block.
  #. Copy the code from above and paste it into the text editor.
  #. Click on the *Run Script* button.
  #. Move your cursor into the 3D Viewport,
     open the :ref:`Operator Search menu <blender_manual:bpy.ops.wm.search_menu>`,
     and type "Simple".
  #. Click on the "Simple Operator" item found in search.

#. Blenderを起動し、スクリプティングワークスペースに切り替えます
#. 新しいテキスト欄を作成するために、テキストエディタで *New* ボタンをクリックします。
#. 上述のコードをコピーし、テキストエディタにペーストします。
#. *Run Script* ボタンをクリックします。
#. カーソルを3Dビューポートに移動し、:ref:`Operator Search menu <blender_manual:bpy.ops.wm.search_menu>` を開き、"Simple"と入力します。
#. 検索結果の" Simple Operator"をクリックします。

.. seealso::
.. comment
   The class members with the ``bl_`` prefix are documented in the API reference :class:`bpy.types.Operator`.

   ``bl_``プレフィックスを持つクラスメンバーは、APIリファレンス :class:`bpy.types.Operator` に文書化されています。

.. note::
.. comment
   The output from the ``main`` function is sent to the terminal;
   in order to see this, be sure to :ref:`use the terminal <use_the_terminal>`.

   ``main``関数の出力はターミナルに送られます。
   これを見るためには、 :ref:`use the terminal <use_the_terminal>` を確認してください。

.. comment
   Example Panel
   -------------

Panelの例
-------------

.. comment
   Panels are registered as a class, like an operator.
   Notice the extra ``bl_`` variables used to set the context they display in.

パネルは、オペレーターと同じく、クラスとして登録されます。
追加の ``bl_`` 変数は、パネルが表示しているコンテキストを設定するために使用されることに注目してください。

.. comment
   .. literalinclude:: __/__/__/__/release/scripts/templates_py/ui_panel_simple.py

.. include:: ../../../../release/scripts/templates_py/ui_panel_simple.py
   :code: python

.. comment
   To run the script:

スクリプトを実行するには:

.. comment
  #. Start Blender and switch to the Scripting workspace.
  #. Click the *New* button in the text editor to create a new text data-block.
  #. Copy the code from above and paste it into the text editor.
  #. Click on the *Run Script* button.

#. Blenderを起動し、スクリプティングワークスペースに切り替えます
#. 新しいテキスト欄を作成するために、テキストエディタで *New* ボタンをクリックします。
#. 上述のコードをコピーし、テキストエディタにペーストします。
#. *Run Script* ボタンをクリックします。

.. comment
   To view the results:

結果を見るには:

.. comment
  #. Select the default cube.
  #. Click on the Object properties icon in the buttons panel (far right; appears as a tiny cube).
  #. Scroll down to see a panel named "Hello World Panel".
  #. Changing the object name also updates *Hello World Panel's* name: field.

#. デフォルトのキューブを選択します
#. ボタンパネルのオブジェクトプロパティアイコンをクリックする。(右端の小さなキューブです)
#. スクロールダウンし、 "Hello World Panel" という名前のパネルを見つけます。
#. オブジェクト名を変更すると、*Hello World Panel*の名前フィールドも更新されます。

.. comment
   Note the row distribution and the label and properties that are defined through the code.

行の配置と、ラベルとプロパティは、コードで定義されることに注意してください。


.. seealso:: :class:`bpy.types.Panel`


Types
=====

.. comment
   Blender defines a number of Python types but also uses Python native types.
   Blender's Python API can be split up into three categories.

Blenderは多くのPython型を定義していますが、Pythonのネイティブ型も使用しています。
BlenderのPython APIは、3つのカテゴリに分けることができます。

Native Types
------------

.. comment
   In simple cases returning a number or a string as a custom type would be cumbersome,
   so these are accessed as normal Python types.

単純なケースでは、数値や文字列をカスタム型としてリターンするのはやりすぎです。
なので、普通のPython型を使用します。

- Blender float, int, boolean -> float, int, boolean
- Blender enumerator -> string

     >>> C.object.rotation_mode = 'AXIS_ANGLE'

- Blender enumerator (multiple) -> set of strings

  .. code-block:: python

     # setting multiple camera overlay guides
     bpy.context.scene.camera.data.show_guide = {'GOLDEN', 'CENTER'}

     # passing as an operator argument for report types
     self.report({'WARNING', 'INFO'}, "Some message!")


Internal Types
--------------

.. comment
   :class:`bpy.types.bpy_struct` is used for Blender data-blocks and collections.
   Also for data that contains its own attributes: collections, meshes, bones, scenes, etc.

:class:`bpy.types.bpy_struct` はBlenderのデータブロックとコレクションに使われます。
collections, meshes, bones, scenesのような自分の属性自身を含むデータにも使われます。

.. comment
   There are two main types that wrap Blender's data, one for data-blocks
   (known internally as ``bpy_struct``), another for properties.

2つのBlenderのデータをラップした主要な型があります。
1つはデータブロック(内部表現では ``bpy_struct``)です。もう1つがプロパティです。

   >>> bpy.context.object
   bpy.data.objects['Cube']

   >>> C.scene.objects
   bpy.data.scenes['Scene'].objects

.. comment
   Note that these types reference Blender's data so modifying them is visible immediately.

これらの型は、Blenderのデータを参照しているので、これらを変更するとすぐに見た目に反映されることに注意してください。

Mathutils Types
---------------

.. comment
   Accessible from :mod:`mathutils` are vectors, quaternions, Euler angles, matrix and color types.
   Some attributes such as :class:`bpy.types.Object.location`,
   :class:`bpy.types.PoseBone.rotation_euler` and :class:`bpy.types.Scene.cursor_location`
   can be accessed as special math types which can be used together and manipulated in various useful ways.

:mod:`mathutils`から利用できるのは、ベクターや、クォータニオン、オイラー角、マトリックス、そしてカラータイプです。
:class:`bpy.types.Object.location` や :class:`bpy.types.PoseBone.rotation_euler`、 :class:`bpy.types.Scene.cursor_location` のような属性は、
一緒に使ったり、様々な便利な方法で操作できる特殊な数学型としてアクセスできます。

.. comment
   Example of a matrix, vector multiplication:

マトリックスとベクターの乗算の例です:

.. code-block:: python

   bpy.context.object.matrix_world @ bpy.context.object.data.verts[0].co

.. comment
   .. note::

      mathutils types keep a reference to Blender's internal data so changes can
      be applied back.

.. note::

   mathutils型は、Blenderの内部データを参照しているので、変更を元に戻すことができます。

   Example:

   .. code-block:: python

      # modifies the Z axis in place.
      bpy.context.object.location.z += 2.0

      # location variable holds a reference to the object too.
      location = bpy.context.object.location
      location *= 2.0

      # Copying the value drops the reference so the value can be passed to
      # functions and modified without unwanted side effects.
      location = bpy.context.object.location.copy()


Animation
=========

.. comment
   There are two ways to add keyframes through Python.

Pythonでキーフレームを追加するには、2つの方法があります。

.. comment
   The first is through key properties directly, which is like inserting a keyframe from the button as a user.
   You can also manually create the curves and keyframe data, then set the path to the property.
   Here are examples of both methods. Both insert a keyframe on the active object's Z axis.

最初の方法は、キープロパティを直接経由する方法です。ユーザーがボタンからキーフレームを追加することと似ています。
カーブとキーフレームデータを手動で作成してから、そのパスをプロパティに設定する方法もあります。
ここに、両方の方法の例があります。両方ともアクティブオブジェクトのZ軸にキーフレームを追加しています。

Simple example:

.. code-block:: python

   obj = bpy.context.object
   obj.location[2] = 0.0
   obj.keyframe_insert(data_path="location", frame=10.0, index=2)
   obj.location[2] = 1.0
   obj.keyframe_insert(data_path="location", frame=20.0, index=2)

Using low-level functions:

.. code-block:: python

   obj = bpy.context.object
   obj.animation_data_create()
   obj.animation_data.action = bpy.data.actions.new(name="MyAction")
   fcu_z = obj.animation_data.action.fcurves.new(data_path="location", index=2)
   fcu_z.keyframe_points.add(2)
   fcu_z.keyframe_points[0].co = 10.0, 0.0
   fcu_z.keyframe_points[1].co = 20.0, 1.0
