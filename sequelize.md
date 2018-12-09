1. sequelize 查询使用sql
```
model.sequelize.query(sql, { type: sequelize.QueryTypes.SELECT });
```
2. 创建时返回自增长id，则在定义是需要
```
 id: {
        autoIncrement: true,
        type: DataTypes.INTEGER,
        primaryKey: true,
    },
```