[TOC]

# ORM与SQLAchemy

## ORM 对象关系映射

### ORM是什么

ORM（Object Relational Mapping）框架采用元数据来描述对象一关系映射细节，元数据一般采用XML格式，并且存放在专门的对象一映射文件中。



![](https://images2017.cnblogs.com/blog/1088183/201712/1088183-20171221220043459-1920885396.png)

### ORM的优点

#### 隐藏数据访问细节

他使得我们的通用数据库交互变得简单易行，并且完全不用考虑该死的SQL语句。快速开发，由此而来。

#### 对于初学者体验好

- 对于初学者来说，它可以使使用更为简便。可以使用模式或模型类是权威的，另一个是自动生成的，这样可以减少错误的数量和锅炉板代码的数量。
- 一般来说，ORM有自己的API来构建查询（例如属性访问），因此不易受SQL注入攻击的影响;

#### 有助于编组以及代码一般更为简单

- 它有助于编组。 ORM通常将单个列的值编组到适当的类型中，这样就不必自己解析/序列化它们。此外，它允许从数据库中检索完全形成的对象，而不需要手动包装自己的对象。

- 查询将返回对象而不是行，因此将能够使用属性访问而不是创建新查询来访问相关对象。可以在需要时直接编写SQL，但对于大多数操作（CRUD），ORM将使与持久对象交互的代码更简单。

- 许多ORM都带有工具，可以检查模式并构建一组模型类，允许与数据库中的对象进行交互。

### ORM的缺点

#### 配置

ORM技术需要配置文件将表模式映射到对象结构中。在大型企业系统中，配置增长非常快，并且变得极难创建和管理。随着业务需求和模型在敏捷环境中不断变化和发展，维护配置也变得乏味且难以维护。

#### 自定义查询

框架提供程序不支持或不推荐映射不适合任何已定义对象的自定义查询的功能。开发人员不得不通过编写adhoc对象和查询，或编写自定义代码来获取所需的数据来找到解决方法。他们可能不得不定期使用存储过程来处理比简单选择更复杂的事情。

#### 特定的绑定

这些框架需要使用在计算机科学行业中未标准化的专有库和专有对象查询语言。这些专有库和查询语言将应用程序绑定到提供程序的特定实现，在需要时很少或根本没有灵活性进行更改，并且没有相互协作的互操作性。

#### 对象查询语言

提供称为对象查询语言的新查询语言，以对对象模型执行查询。它们会自动生成针对数据库的SQL查询，并从流程中抽象出用户。对于面向对象的开发人员来说，这似乎是一个好处，因为他们觉得编写SQL的问题已经解决了。实际问题是这些查询语言不能支持大多数现实世界应用程序所需的一些中高级SQL结构。它们还可以防止开发人员在必要时调整SQL查询。

#### 性能

ORM层使用反射和内省来使用数据库中的数据实例化和填充对象。这些在处理方面是昂贵的操作并且增加了映射操作的性能降级。对象查询被转换为生成未优化的查询，而无需调整它们，从而导致显着的性能损失和数据库管理系统的过载。性能调优SQL几乎是不可能的，因为框架在控制自动生成的SQL方面提供的灵活性很小。

#### 紧耦合性

这种方法在模型对象和数据库模式之间产生了严格的依赖关系。开发人员不希望数据库字段和类字段之间存在一对一的关联。更改数据库模式会在对象模型和映射配置中产生波动影响，反之亦然。

#### 缓存

此方法还需要使用对象缓存和上下文，这些对象缓存和上下文是维护和跟踪对象状态所必需的，并减少缓存数据的数据库往返。如果不在多层实现中维护和同步，这些高速缓存可能在数据准确性和并发性方面产生重大影响。通常需要插入第三方缓存或外部缓存来解决此问题，从而给数据访问层增加了大量负担。

## SQLAchemy

### SQLAchemy是什么

SQLAlchemy是Python编程语言下的一款ORM框架，该框架建立在数据库API之上，使用关系对象映射进行数据库操作，简言之便是：将对象转换成SQL，然后使用数据API执行SQL并获取执行结果。

### SQLAchemy的使用

#### 配置连接数据库的url与连接对象

```python
# 主机的MySQL账号:密码:端口号/数据库
SQLALCHEMY_DATABASE_URI = 'mysql+mysqlconnector://root:mysql@localhost:3306/test2'
# 创建连接对象
db = SQLAlchemy(app)
```

#### 常见的SQLAchemy数据类型

| 类型    | python中的类型对照                               |
| ------- | ------------------------------------------------ |
| integer | int  普通整型，一般32位                          |
| float   | float 浮点数                                     |
| Numeric | decimal.Decimal 普通整数，一般是32位             |
| String  | str 变长字符串                                   |
| Text    | str 变长字符串，对较长或不限长度的字符串做了优化 |
| Unicode | unicode 变长Unicode字符串                        |
| Boolen  | bool 布尔值                                      |
| Date    | datetime.date 时间                               |
| Time    | datetime.datetime：日期和时间                    |

#### 常用的SQLAlchemy列选项

| 选项名      | 说明                         |
| ----------- | ---------------------------- |
| primary_key | True代表主键                 |
| unique      | True代表这列不允许出现重复值 |
| index       | True为这列创建索引，提高效率 |
| nullable    | True，允许有空值             |
| default     | 该列的默认值                 |

#### 创建表格

```python
import random

from .prepare import app, db, model_repr

class Answer(db.Model):
    __tablename__ = 'answers'

    accept_id = db.Column('accept_id', db.Integer, db.ForeignKey(
        'accepts.id', ondelete='cascade'), nullable=False, comment='接受id')
    problem_id = db.Column('problem_id', db.Integer, db.ForeignKey(
        'problems.id', ondelete='cascade'), nullable=False, comment='问题id')
    # answer_id = db.Column('answer_id', db.Integer, db.ForeignKey(
    #     'answers.openid', ondelete='cascade'), nullable=False, comment='回答者id')
    # task_id = db.Column('task_id', db.Integer, db.ForeignKey(
    #     'tasks.id', ondelete='cascade'), nullable=False, comment='任务id')
    answer = db.Column('answer', db.Integer(
    ), nullable=False, server_default='-1', comment='具体答案的选项')

    accept = db.relationship('Accept', back_populates='answers')
    problem = db.relationship('Problem', back_populates='answers')
    # task = db.relationship('Task', back_populates='answers')
    # student = db.relationship('Student', back_populates='answers')

    __table_args__ = (
        db.PrimaryKeyConstraint('accept_id', 'problem_id'),
        db.Index('problem_index', 'problem_id'),
    )

    def __repr__(self):
        return model_repr(self, app.config['ANSWER_JSON_PATTERN'], app.config['ANSWER_JSON_ATTR_ORDER'])
```

#### 常见的SQLAlchemy查询迭代器

| 方法 func    | 说明                                         |
| ------------ | -------------------------------------------- |
| all          | 以列表形式返回查询的所有结果                 |
| first        | 返回查询的第一个结果，如果未查询到，返回None |
| first_or_404 | 返回查询的第一个结果，如果未查询到，返回404  |
| get          | 返回指定主键对应的行，如不存在，返回None     |
| get_or_404   | 返回指定主键对应的行，如不存在，返回404      |
| count        | 返回查询的结果的数量                         |

```python
ret = session.query(Answer).all()  #查询所有
#也可以这样写：
ret = Session.query(Answer.accept,Answer.problem).all()
......
```

#### 常见的SQLAlchemy查询过滤器

| 过滤器    | 说明                                           |
| --------- | ---------------------------------------------- |
| filter    | 把过滤器添加到原查询上，返回一个新查询结果     |
| filter_by | 把等值过滤器添加到原查询上，返回一个新查询     |
| limit     | 使用指定的值限定原查询返回的结果               |
| offset    | 偏移原查询返回的结果，返回一个新查询           |
| order_by  | 根据指定条件对查询结果进行排序，返回一个新查询 |
| group_by  | 根据指定条件对查询结果进行分组，返回一个新查询 |

#### 修改数据

- 直接赋值然后利用session进行commit，删除也是一样的，delete之后commit

**详情可以参考[闲余翻身后端](<https://github.com/sysu-team1/BackEnd/tree/master/db>)**



