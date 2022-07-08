.. _info_overview:

*******************
Python API Overview
*******************

.. comment
    The purpose of this document is to explain how Python and Blender fit together,
    covering some of the functionality that may not be obvious from reading the API references
    and example scripts.

この文書の目的は、どのようにPythonとBlenderがどのように組み合されるかを説明することです。
APIリファレンスとサンプルスクリプトを読んだだけではわからない、いくつかの機能をカバーします。

Python in Blender
=================

.. comment
    Blender has an embedded Python interpreter which is loaded when Blender is started
    and stays active while Blender is running. This interpreter runs scripts to draw the user interface
    and is used for some of Blender's internal tools as well.

Blenderは組み込みのPythonインタプリタを持っています。
インタプリタはBlender開始時にロードされ、Blender実行中はアクティブになっています。
このインタプリタはユーザーインターフェイスを描画するためのスクリプトを実行し、内部ツールの一部にも使用されています。

.. comment
    Blender's embedded interpreter provides a typical Python environment, so code from tutorial
    on how to write Python scripts can also be run with Blender's interpreter. Blender provides its
    Python modules, such as :mod:`bpy` and :mod:`mathutils`, to the embedded interpreter so they can
    be imported into a script and give access to Blender's data, classes, and functions.
    Scripts that deal with Blender data will need to import the modules to work.

Blenderの組み込みインタプリタは、典型的なPython環境を提供します。
そのため、Pythonスクリプトの書き方のチュートリアルにあるコードも、Blenderのインタプリタで実行できます。
Blenderは、 :mod:`bpy` や :mod:`mathutils` といった Pythonモジュールを組み込みインタプリタを提供します。
これらのモジュールはスクリプトにインポートされ、Blenderのデータやクラス、関数へのアクセスを提供します。
Blenderのデータを扱うスクリプトは、これらのモジュールをインポートする必要があります。

.. comment
    Here is a simple example which moves a vertex attached to an object named "Cube":

ここに、"Cube"という名前のオブジェクトの頂点を移動させる簡単なサンプルがあります。

.. code-block:: python

   import bpy
   bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0

.. comment
    This modifies Blender's internal data directly.
    When you run this in the interactive console you will see the 3D Viewport update.

このサンプルはBlenderの内部データを直接変更します。
インタラクティブコンソールでこのサンプルを実行すると、3Dビューポートが更新されることを確認できます。


The Default Environment
=======================

.. comment
    When developing your own scripts it may help to understand how Blender sets up its Python environment.
    Many Python scripts come bundled with Blender and can be used as a reference
    because they use the same API that script authors write tools in.
    Typical usage for scripts include: user interface, import/export,
    scene manipulation, automation, defining your own tool set and customization.

自身のスクリプトを開発する際は、BlenderがどのようにPython環境をセットアップするかを理解しておくと役に立つかもしれません。
たくさんのPythonスクリプトがBlenderに付属しており、
ツールを開発する時に使用するAPIと同じものが使われているため、リファレンスとしても利用されます。
スクリプトの典型的な使用法は、ユーザーインターフェイスやインポート/エクスポート、シーン操作、自動化、独自のツールの定義、カスタマイズなどです。

.. comment
    On startup Blender scans the ``scripts/startup/`` directory for Python modules and imports them.
    The exact location of this directory depends on your installation.
    See the :ref:`directory layout docs <blender_manual:blender-directory-layout>`.

起動時にBlenderは ``scripts/startup/`` ディレクトリのPythonモジュールをスキャンし、インポートします。
そのディレクトリの実際の場所は、あなたがインストールした環境に依存します。
 :ref:`directory layout docs <blender_manual:blender-directory-layout>` を参照してください。

Script Loading
==============

.. comment
    This may seem obvious, but it is important to note the difference between
    executing a script directly and importing a script as a module.

これは当たり前のように思えます。しかし、スクリプトを直接実行することと
スクリプトをモジュールとしてインポートすることの違いを知ることは重要です。

.. comment
    Extending Blender by executing a script directly means the classes that the script defines
    remain available inside Blender after the script finishes execution.
    Using scripts this way makes future access to their classes
    (to unregister them for example) more difficult compared to importing the scripts as modules.
    When a script is imported as a module, its class instances will remain
    inside the module and can be accessed later on by importing that module again.

スクリプトの実行によりBlenderを拡張するということは、
スクリプトで定義したクラスが、スクリプト実行完了後もBlender内で利用可能であることを意味します。
この方法でスクリプトを使用すると、モジュールとしてスクリプトをインポートする方法と比べて、
将来これらのクラスにアクセスすること(例えば登録解除する)が難しくなります。
スクリプトがモジュールとしてインポートされた時は、そのクラスのインスタンスはモジュールの中に残ります。
そして、あとで、モジュールを再度インポートすることによりアクセスできるようになります。

.. comment
    For this reason it is preferable to avoid directly executing scripts that extend Blender by registering classes.

そのため、クラスを登録してBlenderを拡張するようなスクリプトを直接実行することは避けた方が良いでしょう。

.. comment
    Here are some ways to run scripts directly in Blender:

直接Blender内でスクリプトを実行する方法がいくつかあります:

.. comment
    - Loaded in the text editor and press *Run Script*.
    - Typed or pasted into the interactive console.
    - Execute a Python file from the command line with Blender, e.g:

- テキストエディタにスクリプトをロードし、 *Run Script* ボタンを押す
- インタラクティブコンソールに入力または貼り付けする
- コマンドラインでBlenderとともにPythonファイルを実行する。 例:

  .. code-block:: sh

     blender --python /home/me/my_script.py

.. comment
    To run as modules:

モジュールとして実行する:

.. comment
  - The obvious way, ``import some_module`` command from the text editor or interactive console.
  - Open as a text data-block and check the *Register* option, this will load with the blend-file.
  - Copy into one of the directories ``scripts/startup``, where they will be automatically imported on startup.
  - Define as an add-on, enabling the add-on will load it as a Python module.

- 簡単な方法は ``import some_module`` コマンドをテキストエディタまたはインタラクティブコンソールで実行する
- *Register* のチェックを付けて、テキストデータブロックとして開きます。これにより、blendファイルといっしょにロードされる
- 起動時に自動的にインポートされる ``scripts/startup`` ディレクトリのいずれにコピーする
- アドオンとして定義し、アドオンを有効にするとモジュールとしてロードされる

Add-ons
-------

.. comment
    Some of Blender's functionality is best kept optional,
    alongside scripts loaded at startup there are add-ons which are kept in their own directory ``scripts/addons``,
    They are only loaded on startup if selected from the user preferences.

いくつかのBlenderの機能はオプションのままがベストです。
起動時に読み込まれるスクリプト以外にアドオンがあり、``scripts/addons`` ディレクトリに格納されています。
それらは、ユーザープリファレンスで選択された場合のみ、起動時に読み込まれます。

.. comment
    The only difference between add-ons and built-in Python modules is that add-ons must contain a ``bl_info`` variable
    which Blender uses to read metadata such as name, author, category and project link.
    The User Preferences add-on listing uses ``bl_info`` to display information about each add-on.
    `See Add-ons <https://wiki.blender.org/index.php/Dev:Py/Scripts/Guidelines/Addons>`__
    for details on the ``bl_info`` dictionary.

アドオンとビルトインPythonモジュールとの唯一の違いは、アドオンは ``bl_info`` 変数を含む必要があることだけです。
この変数は、名前や作者、カテゴリやプロジェクトのリンクなどのメタデータをBlenderが読むために使われます。
ユーザープリファレンスのアドオンリストは、アドオンの情報を表示するために ``bl_info`` を使います。

``bl_info`` ディクショナリーの詳細は、 `Add-ons <https://wiki.blender.org/index.php/Dev:Py/Scripts/Guidelines/Addons> を参照`__


Integration through Classes
===========================

.. comment
    Running Python scripts in the text editor is useful for testing but you'll
    want to extend Blender to make tools accessible like other built-in functionality.

Pythonスクリプトをテキストエディタで実行することは、動作確認には便利ですが、
Blenderを拡張し、他のビルトイン機能のようにアクセスできるツールを作成したいと思うでしょう。

.. comment
    The Blender Python API allows integration for:

統合が許可されたBlender Python API は以下:

- :class:`bpy.types.Panel`
- :class:`bpy.types.Menu`
- :class:`bpy.types.Operator`
- :class:`bpy.types.PropertyGroup`
- :class:`bpy.types.KeyingSet`
- :class:`bpy.types.RenderEngine`

.. comment
    This is intentionally limited. Currently, for more advanced features such as mesh modifiers,
    object types, or shader nodes, C/C++ must be used.

これは意図して制限されています。現在のところ、
メッシュモディファイアやオブジェクトタイプ、シェーダーノードなど高度な機能を使用する場合は、C/C++を利用する必要があります。

.. comment
    For Python integration Blender defines methods which are common to all types.
    This works by creating a Python subclass of a Blender class which contains variables and functions
    specified by the parent class which are predefined to interface with Blender.

Python統合のために、Blenderは全タイプで共通のメソッドを定義します。
これはBlenderクラスのサブクラスを作成することによって動作します。
BlenderクラスはBlenderとのインターフェイスとしてあらかじめ定義された親クラスで指定された変数や関数を含んでいます。

.. comment
    For example:

例:

.. code-block:: python

   import bpy
   class SimpleOperator(bpy.types.Operator):
       bl_idname = "object.simple_operator"
       bl_label = "Tool Name"

       def execute(self, context):
           print("Hello World")
           return {'FINISHED'}

   bpy.utils.register_class(SimpleOperator)

.. comment
    First note that it defines a subclass as a member of :mod:`bpy.types`,
    this is common for all classes which can be integrated with Blender and
    is used to distinguish an Operator from a Panel when registering.

まず、:mod:`bpy.types`のメンバーであるクラスのサブクラスを定義します。
これはBelnderに統合できる全てのクラスに共通です。
そして、登録時に、Operator か Panel を区別するために使用されます。

.. comment
    Both class properties start with a ``bl_`` prefix.
    This is a convention used to distinguish Blender properties from those you add yourself.
    Next see the execute function, which takes an instance of the operator and the current context.
    A common prefix is not used for functions.
    Lastly the register function is called, this takes the class and loads it into Blender. See `Class Registration`_.

クラスプロパティはどちらも ``bl_`` プレフィックスではじまっています。
これは規約です。Blenderのプロパティなのか、あなた自身が追加したプロパティか識別するために使われます。
次に、execute関数をみていきましょう。この関数はオペレーターのインスタンスとカレントコンテキストを引数にとります。
共通のプレフィックスは関数には使われません。
最後に、register関数が呼ばれます。これは、クラスを引数にとり、Blenderにクラスをロードします。

参照: `Class Registration`_

.. comment
    Regarding inheritance, Blender doesn't impose restrictions on the kinds of class inheritance used,
    the registration checks will use attributes and functions defined in parent classes.

継承に関して、Blenderは継承に使えるクラスの種類に制限を課しません。
登録チェックには、親クラスで定義された属性や関数が使用されます。

.. comment
    Class mix-in example:

クラスのmix-inの例:

.. code-block:: python

   import bpy
   class BaseOperator:
       def execute(self, context):
           print("Hello World BaseClass")
           return {'FINISHED'}

   class SimpleOperator(bpy.types.Operator, BaseOperator):
       bl_idname = "object.simple_operator"
       bl_label = "Tool Name"

   bpy.utils.register_class(SimpleOperator)

.. comment
    Notice these classes don't define an ``__init__(self)`` function.
    While ``__init__()`` and ``__del__()`` will be called if defined,
    the class instances lifetime only spans the execution.
    So a panel for example will have a new instance for every redraw,
    for this reason there is rarely a cause to store variables in the panel instance.
    Instead, persistent variables should be stored in Blender's data
    so that the state can be restored when Blender is restarted.

これらのクラスが ``__init__(self)`` を定義していないことに注意してください。
``__init__()`` や ``__del__()`` が定義されていれば呼ばれますが、クラスインスタンスのライフタイムは実行時に限られます。
そのため、パネルの例では、再描画される度に新しいインスタンスを持ちます。
このため、パネルインスタンスに変数を保持するケースはほとんどありません。
その代りに、永続的な変数はBlenderのデータに保持されるべきです。
そうすれば、Blenderを再起動したときに状態が復元できます。

.. comment
    .. note::

    Modal operators are an exception, keeping their instance variable as Blender runs, see modal operator template.

.. note::

    モーダルオペレーターは例外です。Blender実行時に、それらのインスタンス変数を保持する必要があります。モーダルオペレーターのテンプレートを参照ください。

.. comment
    So once the class is registered with Blender, instancing the class and calling the functions is left up to Blender.
    In fact you cannot instance these classes from the script as you would expect with most Python API's.
    To run operators you can call them through the operator API, e.g:

一度、クラスをBlenderに登録したら、クラスのインスタンス化や関数の呼び出しはBlenderに任されます。
実際、ほとんどのPython APIで期待されるように、スクリプトからこれらのクラスのインスタンスを生成できません。
オペレーターを実行させるには、オペレーターAPIを経由して呼び出す必要があります。 例:

.. code-block:: python

   import bpy
   bpy.ops.object.simple_operator()

.. comment
    User interface classes are given a context in which to draw, buttons, window, file header, toolbar, etc.,
    then they are drawn when that area is displayed so they are never called by Python scripts directly.

ユーザーインターフェイスクラスには、ボタンやウィンドウ、ファイルヘッダやツールバーなど、描画すべきコンテキストが渡されます。
それから、領域が表示される時に描画されます。そのため、Pythonスクリプトから直接呼び出されることはありません。

.. _info_overview_registration:

Registration
============

Module Registration
-------------------

.. comment
    Blender modules loaded at startup require ``register()`` and ``unregister()`` functions.
    These are the *only* functions that Blender calls from your code, which is otherwise a regular Python module.

起動時にロードされるBlenderモジュールは、 関数 ``register()`` と ``unregister()`` が必要です。
これらは、あなたのコードからBlenderが呼び出す *唯一の* 関数です。それ以外は通常のPythonモジュールです。

.. comment
    A simple Blender Python module can look like this:

簡単なBlender Pythonモジュールは、このようになります:

.. code-block:: python

   import bpy

   class SimpleOperator(bpy.types.Operator):
       """ See example above """

   def register():
       bpy.utils.register_class(SimpleOperator)

   def unregister():
       bpy.utils.unregister_class(SimpleOperator)

   if __name__ == "__main__":
       register()

.. comment
    These functions usually appear at the bottom of the script containing class registration sometimes adding menu items.
    You can also use them for internal purposes setting up data for your own tools but take care
    since register won't re-run when a new blend-file is loaded.

これらの関数は、通常、クラス登録やメニュー追加を含むスクリプトの一番下に登場します。
自身のツールのデータセットアップなどの内部目的のために、これらの関数を利用することもできます。
しかし、新しいblendファイルがロードされた時に、register は再実行されないので注意しましょう。

.. comment
    The register/unregister calls are used so it's possible to toggle add-ons and reload scripts while Blender runs.
    If the register calls were placed in the body of the script, registration would be called on import,
    meaning there would be no distinction between importing a module or loading its classes into Blender.
    This becomes problematic when a script imports classes from another module
    making it difficult to manage which classes are being loaded and when.

register と unregister は、Blenderの実行時に、アドオンの切り替えやスクリプトのリロードを可能にするために使用されます。
もし、registerの呼び出しがスクリプトに本体に書かれていたら、インポート時に登録されます。
これは、モジュールのインポートとBlenderへのクラスのロードの間に違いがないことを意味します。
これは、スクリプトが他のモジュールからクラスをインポートする時に問題になります。
どのクラスがいつロードされるかを管理することが難しくなります。

.. comment
    The last two lines are only for testing:

最後の2行はテスト用です:

.. code-block:: python

   if __name__ == "__main__":
       register()

.. comment
    This allows the script to be run directly in the text editor to test changes.
    This ``register()`` call won't run when the script is imported as a module
    since ``__main__`` is reserved for direct execution.

これにより、テキストエディタでスクリプトが直接実行し、変更内容をテストできます。
``__main__`` は直接実行のために予約されているので、
この ``register()`` 呼び出しは、スクリプトがモジュールとしてインポートされる時には実行されません。


Class Registration
------------------

.. comment
    Registering a class with Blender results in the class definition being loaded into Blender,
    where it becomes available alongside existing functionality.
    Once this class is loaded you can access it from :mod:`bpy.types`,
    using the ``bl_idname`` rather than the classes original name.

Blenderにクラスを登録すると、クラス定義がBlenderに読み込まれます。
そこで、既存の機能と一緒に利用できるようになります。
一度、クラスがロードされたら、 :mod:`bpy.types` からアクセスできます。
 この時、クラスのオリジナル名ではなく ``bl_idname`` を使います。

.. comment
    .. note::

    There are some exceptions to this for class names which aren't guarantee to be unique.
    In this case use: :func:`bpy.types.Struct.bl_rna_get_subclass_py`.

.. note::

    これにはいくつか例外があります。クラス名はユニークであることを保証しません。
    その場合は、 :func:`bpy.types.Struct.bl_rna_get_subclass_py` を使います。

.. comment
    When loading a class, Blender performs sanity checks making sure all required properties and functions are found,
    that properties have the correct type, and that functions have the right number of arguments.

クラスをロードするときに、Blenderは整合性チェックを実施、
全ての必要なプロパティと関数が存在するか、プロパティが正しい型か、関数は正しい数の引数を取るかなどを確認します。

.. comment
    Mostly you will not need concern yourself with this but if there is a problem
    with the class definition it will be raised on registering:

ほとんどの場合、気にかける必要はないでしょう。しかし、クラス定義に問題があれば登録時に例外が発生します。

.. comment
    Using the function arguments ``def execute(self, context, spam)``, will raise an exception:

``def execute(self, context, spam)`` のような引数を使用した場合、例外が発生するでしょう:

``ValueError: expected Operator, SimpleOperator class "execute" function to have 2 args, found 3``

.. comment
    Using ``bl_idname = 1`` will raise:

``bl_idname = 1`` の場合、例外が発生するでしょう:

``TypeError: validating class error: Operator.bl_idname expected a string type, not int``


Inter-Class Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^

.. comment
    When customizing Blender you may want to group your own settings together,
    after all, they will likely have to co-exist with other scripts.
    To group these properties classes need to be defined,
    for groups within groups or collections within groups
    you can't avoid having to deal with the order of registration/unregistration.

Blenderをカスタマイズする時、自身の設定をいっしょにまとめたいと思うかもしれません。
やはり、他のスクリプトと共存していかなければなりません。
これらのプロパティをグループ化するためには、クラスを定義する必要があります。
グループ内のグループやグループのコレクションに対しては、登録/解除の順番に悩まされることは避けれれません。

.. comment
    Custom properties groups are themselves classes which need to be registered.

カスタムプロパティグループは、それら自身が登録が必要なクラスです。

.. comment
    For example, if you want to store material settings for a custom engine:

例えば、もし、マテリアル設定をカスタムエンジンのために保存したい場合:

.. code-block:: python

   # Create new property
   # bpy.data.materials[0].my_custom_props.my_float
   import bpy

   class MyMaterialProps(bpy.types.PropertyGroup):
       my_float: bpy.props.FloatProperty()

   def register():
       bpy.utils.register_class(MyMaterialProps)
       bpy.types.Material.my_custom_props: bpy.props.PointerProperty(type=MyMaterialProps)

   def unregister():
       del bpy.types.Material.my_custom_props
       bpy.utils.unregister_class(MyMaterialProps)

   if __name__ == "__main__":
       register()

.. comment
    .. note::

    The class **must be** registered before being used in a property, failing to do so will raise an error:

.. note::

    クラスは、プロパティを使用する前に登録される必要があります。そうしなければエラーが発生するでしょう:

   ``ValueError: bpy_struct "Material" registration error: my_custom_props could not register``


.. code-block:: python

   # Create new property group with a sub property
   # bpy.data.materials[0].my_custom_props.sub_group.my_float
   import bpy

   class MyMaterialSubProps(bpy.types.PropertyGroup):
       my_float: bpy.props.FloatProperty()

   class MyMaterialGroupProps(bpy.types.PropertyGroup):
       sub_group: bpy.props.PointerProperty(type=MyMaterialSubProps)

   def register():
       bpy.utils.register_class(MyMaterialSubProps)
       bpy.utils.register_class(MyMaterialGroupProps)
       bpy.types.Material.my_custom_props: bpy.props.PointerProperty(type=MyMaterialGroupProps)

   def unregister():
       del bpy.types.Material.my_custom_props
       bpy.utils.unregister_class(MyMaterialGroupProps)
       bpy.utils.unregister_class(MyMaterialSubProps)

   if __name__ == "__main__":
       register()

.. comment
    .. important::

    The lower most class needs to be registered first and that ``unregister()`` is a mirror of ``register()``.

.. important:: ::

    最下位のクラスが最初に登録される必要があり、解除はその逆です。

Manipulating Classes
^^^^^^^^^^^^^^^^^^^^

.. comment
    Properties can be added and removed as Blender runs,
    normally done on register or unregister but for some special cases
    it may be useful to modify types as the script runs.

プロパティはBlenderの実行中に追加、削除できます。
通常、登録時または解除時に行なわれます。
しかし、いくつかの特殊なケースの場合、スクリプト実行時に型を変更することが便利かもしれません。

.. comment
    For example:

例:

.. code-block:: python

   # add a new property to an existing type
   bpy.types.Object.my_float: bpy.props.FloatProperty()
   # remove
   del bpy.types.Object.my_float

.. comment
    This works just as well for ``PropertyGroup`` subclasses you define yourself.

これは、自身が定義した ``PropertyGroup`` サブクラスでも、同様に機能します。

.. code-block:: python

   class MyPropGroup(bpy.types.PropertyGroup):
       pass
   MyPropGroup.my_float: bpy.props.FloatProperty()

.. comment
    This is equivalent to:

これは以下と同じです:

.. code-block:: python

   class MyPropGroup(bpy.types.PropertyGroup):
       my_float: bpy.props.FloatProperty()


Dynamic Class Definition (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. comment
    In some cases the specifier for data may not be in Blender, for example a external render engines shader definitions,
    and it may be useful to define them as types and remove them on the fly.

例えば、外部レンダリングエンジンのシェーダー定義など、Blenderに該当データがない場合があります。
そして、それらのデータを型として、その場で定義したり、削除したりすることが便利な場合があります。

.. code-block:: python

   for i in range(10):
       idname = "object.operator_%d" % i

       def func(self, context):
           print("Hello World", self.bl_idname)
           return {'FINISHED'}

       opclass = type("DynOp%d" % i,
                      (bpy.types.Operator, ),
                      {"bl_idname": idname, "bl_label": "Test", "execute": func},
                      )
       bpy.utils.register_class(opclass)

.. comment
    .. note::

    ``type()`` is called to define the class.
    This is an alternative syntax for class creation in Python, better suited to constructing classes dynamically.

.. note::

    ``type()`` は、クラスを定義するために呼び出されます。
    これは、Pythonのクラス生成の代替構文です。
    クラスを動的に作成するときに適しています。

To call the operators from the previous example:

   >>> bpy.ops.object.operator_1()
   Hello World OBJECT_OT_operator_1
   {'FINISHED'}

   >>> bpy.ops.object.operator_2()
   Hello World OBJECT_OT_operator_2
   {'FINISHED'}
